#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

struct Point
{
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};

vector<vector<int>> bfs(const vector<string> &grid, const Point &start)
{
    int n = grid.size();
    int m = grid[0].size();

    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<Point> q;

    dist[start.x][start.y] = 0;
    q.push(start);

    while (!q.empty())
    {
        Point curr = q.front();
        q.pop();

        for (int dir = 0; dir < 4; dir++)
        {
            int nx = curr.x + dx[dir];
            int ny = curr.y + dy[dir];

            if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                grid[nx][ny] != '#' && dist[nx][ny] == -1)
            {
                dist[nx][ny] = dist[curr.x][curr.y] + 1;
                q.push(Point(nx, ny));
            }
        }
    }

    return dist;
}

vector<Point> reconstruct_path(const vector<string> &grid,
                               const vector<vector<int>> &dist,
                               const Point &start,
                               const Point &end)
{
    vector<Point> path;

    if (dist[end.x][end.y] == -1)
        return path;

    Point curr = end;
    path.push_back(curr);

    while (!(curr.x == start.x && curr.y == start.y))
    {
        for (int dir = 0; dir < 4; dir++)
        {
            int nx = curr.x + dx[dir];
            int ny = curr.y + dy[dir];

            if (nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size() &&
                dist[nx][ny] != -1 && dist[nx][ny] == dist[curr.x][curr.y] - 1)
            {
                curr = Point(nx, ny);
                path.push_back(curr);
                break;
            }
        }
    }

    reverse(path.begin(), path.end());
    return path;
}

int main()
{
    vector<string> grid;
    string line;

    while (getline(cin, line) && !line.empty())
    {
        grid.push_back(line);
    }

    int n = grid.size();
    int m = grid[0].size();

    Point A(-1, -1), B(-1, -1), F(-1, -1);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (grid[i][j] == 'A')
            {
                A = Point(i, j);
            }
            else if (grid[i][j] == 'B')
            {
                B = Point(i, j);
            }
            else if (grid[i][j] == 'F')
            {
                F = Point(i, j);
            }
        }
    }

    vector<vector<int>> dist_A = bfs(grid, A);
    vector<vector<int>> dist_B = bfs(grid, B);
    vector<vector<int>> dist_F = bfs(grid, F);

    Point M = F;
    int min_total = INT_MAX;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (grid[i][j] != '#' &&
                dist_A[i][j] != -1 &&
                dist_B[i][j] != -1 &&
                dist_F[i][j] != -1)
            {

                int total = dist_A[i][j] + dist_B[i][j] + dist_F[i][j];
                if (total < min_total)
                {
                    min_total = total;
                    M = Point(i, j);
                }
            }
        }
    }

    vector<Point> path_A_to_M = reconstruct_path(grid, dist_A, A, M);
    vector<Point> path_B_to_M = reconstruct_path(grid, dist_B, B, M);
    vector<Point> path_M_to_F = reconstruct_path(grid, dist_F, M, F);

    vector<string> result = grid;

    for (size_t i = 1; i < path_A_to_M.size(); i++)
    {
        Point p = path_A_to_M[i];
        if (!(p.x == M.x && p.y == M.y))
        {
            result[p.x][p.y] = 'o';
        }
    }

    for (size_t i = 1; i < path_B_to_M.size(); i++)
    {
        Point p = path_B_to_M[i];
        if (!(p.x == M.x && p.y == M.y))
        {
            result[p.x][p.y] = 'o';
        }
    }

    for (size_t i = 0; i < path_M_to_F.size(); i++)
    {
        Point p = path_M_to_F[i];
        if (!(p.x == M.x && p.y == M.y) && !(p.x == F.x && p.y == F.y))
        {
            result[p.x][p.y] = 'o';
        }
    }

    if (!(M.x == F.x && M.y == F.y))
    {
        result[M.x][M.y] = 'M';
    }

    cout << "Оптимальная точка встречи M: (" << M.x << ", " << M.y << ")" << endl;
    cout << "Суммарный путь: " << min_total << endl
         << endl;

    for (const string &row : result)
    {
        cout << row << endl;
    }

    return 0;
}