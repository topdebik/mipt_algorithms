#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <tuple>

using namespace std;

struct Cell
{
    int x, y;
    int prev_x, prev_y;
};

const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, 1, -1};

pair<vector<vector<char>>, bool> findShortestPath(const string &maze_str)
{
    vector<string> maze;
    string temp;
    for (char c : maze_str)
    {
        if (c == '\n')
        {
            if (!temp.empty())
            {
                maze.push_back(temp);
                temp.clear();
            }
        }
        else
        {
            temp += c;
        }
    }
    if (!temp.empty())
    {
        maze.push_back(temp);
    }

    int n = maze.size();
    int m = maze[0].size();

    int start_x = -1, start_y = -1;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (maze[i][j] == 'S')
            {
                start_x = i;
                start_y = j;
                break;
            }
        }
        if (start_x != -1)
            break;
    }

    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<Cell>> parent(n, vector<Cell>(m, {-1, -1, -1, -1}));
    queue<pair<int, int>> q;

    q.push({start_x, start_y});
    visited[start_x][start_y] = true;

    int exit_x = -1, exit_y = -1;
    bool found_exit = false;

    while (!q.empty() && !found_exit)
    {
        auto [x, y] = q.front();
        q.pop();

        if ((x == 0 || x == n - 1 || y == 0 || y == m - 1) &&
            (maze[x][y] == '.' || maze[x][y] == 'S'))
        {
            exit_x = x;
            exit_y = y;
            found_exit = true;
            break;
        }

        for (int d = 0; d < 4; d++)
        {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                !visited[nx][ny] && maze[nx][ny] != '#')
            {
                visited[nx][ny] = true;
                parent[nx][ny] = {nx, ny, x, y};
                q.push({nx, ny});
            }
        }
    }

    if (!found_exit)
    {
        vector<vector<char>> result(n, vector<char>(m));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                result[i][j] = maze[i][j];
            }
        }
        return {result, false};
    }

    vector<vector<char>> result_maze(n, vector<char>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            result_maze[i][j] = maze[i][j];
        }
    }

    int cur_x = exit_x, cur_y = exit_y;
    while (!(cur_x == start_x && cur_y == start_y))
    {
        if (result_maze[cur_x][cur_y] != 'S')
        {
            result_maze[cur_x][cur_y] = 'o';
        }
        auto prev = parent[cur_x][cur_y];
        cur_x = prev.prev_x;
        cur_y = prev.prev_y;
    }

    return {result_maze, true};
}

void printMaze(const vector<vector<char>> &maze)
{
    for (const auto &row : maze)
    {
        for (char c : row)
        {
            cout << c;
        }
        cout << endl;
    }
}

int main()
{
    string maze_str =
        "#####\n"
        "#.S.#\n"
        "###.#\n"
        "#...#\n"
        "#.###\n";

    auto [result_maze, found] = findShortestPath(maze_str);

    if (found)
    {
        cout << "Кратчайший путь найден:" << endl;
    }
    else
    {
        cout << "Выход не найден:" << endl;
    }

    printMaze(result_maze);

    return 0;
}