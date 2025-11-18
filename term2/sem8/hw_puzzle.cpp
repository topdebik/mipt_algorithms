#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

// Класс для представления состояния доски
class Board
{
public:
    vector<vector<int>> tiles;
    int zero_row, zero_col;
    int g_cost;  // стоимость от начального состояния
    int h_cost;  // эвристическая стоимость до целевого состояния
    string path; // последовательность ходов

    Board(const vector<vector<int>> &t, int zr, int zc, int g, const string &p = "")
        : tiles(t), zero_row(zr), zero_col(zc), g_cost(g), path(p)
    {
        h_cost = calculateHeuristic();
    }

    // Вычисление эвристики (манхэттенское расстояние)
    int calculateHeuristic() const
    {
        int distance = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                int value = tiles[i][j];
                if (value != 0)
                {
                    int target_row = (value - 1) / 4;
                    int target_col = (value - 1) % 4;
                    distance += abs(i - target_row) + abs(j - target_col);
                }
            }
        }
        return distance;
    }

    // Общая стоимость
    int getCost() const
    {
        return g_cost + h_cost;
    }

    // Проверка, является ли состояние целевым
    bool isGoal() const
    {
        vector<vector<int>> goal = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0}};
        return tiles == goal;
    }

    // Генерация возможных ходов
    vector<Board> getNeighbors() const
    {
        vector<Board> neighbors;
        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        vector<char> direction_chars = {'U', 'D', 'L', 'R'};

        for (int i = 0; i < 4; i++)
        {
            int new_row = zero_row + directions[i].first;
            int new_col = zero_col + directions[i].second;

            if (new_row >= 0 && new_row < 4 && new_col >= 0 && new_col < 4)
            {
                vector<vector<int>> new_tiles = tiles;
                swap(new_tiles[zero_row][zero_col], new_tiles[new_row][new_col]);

                string new_path = path + direction_chars[i];
                neighbors.emplace_back(new_tiles, new_row, new_col, g_cost + 1, new_path);
            }
        }

        return neighbors;
    }

    // Преобразование доски в строку для использования в хэш-таблице
    string toString() const
    {
        string result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result += to_string(tiles[i][j]) + " ";
            }
        }
        return result;
    }

    // Оператор для сравнения в priority_queue
    bool operator>(const Board &other) const
    {
        return getCost() > other.getCost();
    }
};

// Функция для проверки разрешимости головоломки
bool isSolvable(const vector<vector<int>> &tiles)
{
    vector<int> sequence;
    int zero_row = -1;

    // Преобразование в линейную последовательность
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tiles[i][j] != 0)
            {
                sequence.push_back(tiles[i][j]);
            }
            else
            {
                zero_row = i;
            }
        }
    }

    // Подсчет инверсий
    int inversions = 0;
    for (int i = 0; i < sequence.size(); i++)
    {
        for (int j = i + 1; j < sequence.size(); j++)
        {
            if (sequence[i] > sequence[j])
            {
                inversions++;
            }
        }
    }

    // Правило разрешимости для головоломки 4x4:
    // Если пустая клетка находится на четной строке снизу (счет с 1), то количество инверсий должно быть нечетным
    // Если пустая клетка находится на нечетной строке снизу, то количество инверсий должно быть четным
    return (inversions % 2 == 0) == ((4 - zero_row) % 2 == 1);
}

// Функция решения головоломки
string solvePuzzle(const vector<vector<int>> &initial)
{
    // Находим позицию нуля
    int zero_row = -1, zero_col = -1;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (initial[i][j] == 0)
            {
                zero_row = i;
                zero_col = j;
                break;
            }
        }
    }

    // Проверяем разрешимость
    if (!isSolvable(initial))
    {
        return "No solution exists";
    }

    // Создаем начальное состояние
    Board start(initial, zero_row, zero_col, 0);

    // Приоритетная очередь для A*
    priority_queue<Board, vector<Board>, greater<Board>> open_set;
    open_set.push(start);

    // Множество посещенных состояний
    unordered_set<string> closed_set;

    // Максимальная глубина
    const int MAX_DEPTH = 60;

    while (!open_set.empty())
    {
        Board current = open_set.top();
        open_set.pop();

        string current_str = current.toString();

        // Пропускаем, если уже посещали
        if (closed_set.find(current_str) != closed_set.end())
        {
            continue;
        }

        // Проверяем максимальную глубину
        if (current.g_cost > MAX_DEPTH)
        {
            continue;
        }

        // Проверяем, является ли состояние целевым
        if (current.isGoal())
        {
            return current.path;
        }

        closed_set.insert(current_str);

        // Генерируем соседние состояния
        vector<Board> neighbors = current.getNeighbors();
        for (const Board &neighbor : neighbors)
        {
            if (closed_set.find(neighbor.toString()) == closed_set.end())
            {
                open_set.push(neighbor);
            }
        }
    }

    return "No solution found within depth limit";
}

int main()
{
    vector<vector<int>> initial(4, vector<int>(4));

    cout << "Enter initial board configuration (4x4):" << endl;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cin >> initial[i][j];
        }
    }

    string solution = solvePuzzle(initial);

    if (solution == "No solution exists" || solution == "No solution found within depth limit")
    {
        cout << solution << endl;
    }
    else
    {
        cout << "Solution: " << solution << endl;
        cout << "Number of moves: " << solution.length() << endl;
    }

    return 0;
}