#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;

    vector<vector<char>> grid(n, vector<char>(m));
    queue<pair<int, int>> q;
    vector<vector<int>> dist(n, vector<int>(m, -1));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> grid[i][j];
            if (grid[i][j] == 'W')
            {
                q.push({i, j});
                dist[i][j] = 0;
            }
        }
    }

    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int maxTime = 0;

    while (!q.empty())
    {
        auto [x, y] = q.front();
        q.pop();

        for (int d = 0; d < 4; d++)
        {
            int nx = x + directions[d][0];
            int ny = y + directions[d][1];

            if (nx >= 0 && nx < n && ny >= 0 && ny < m && dist[nx][ny] == -1 && grid[nx][ny] == 'L')
            {
                dist[nx][ny] = dist[x][y] + 1;
                maxTime = max(maxTime, dist[nx][ny]);
                q.push({nx, ny});
            }
        }
    }

    cout << maxTime << endl;

    return 0;
}