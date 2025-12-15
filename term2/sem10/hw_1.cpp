#include <bits/stdc++.h>
using namespace std;

int main()
{
    vector<string> g;
    string line;

    while (getline(cin, line))
    {
        if (line.empty())
            break;
        g.push_back(line);
    }

    int n = g.size();
    if (n == 0)
        return 0;
    int m = g[0].size();

    int si = -1, sj = -1, keys = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (g[i][j] == '@')
            {
                si = i;
                sj = j;
            }
            if (g[i][j] >= 'a' && g[i][j] <= 'f')
            {
                keys = max(keys, g[i][j] - 'a' + 1);
            }
        }
    }

    int need = (1 << keys) - 1;

    vector<vector<vector<int>>> d(n, vector<vector<int>>(m, vector<int>(64, -1)));

    queue<tuple<int, int, int>> q;
    q.push({si, sj, 0});
    d[si][sj][0] = 0;

    int di[] = {-1, 1, 0, 0};
    int dj[] = {0, 0, -1, 1};

    while (!q.empty())
    {
        auto [i, j, mask] = q.front();
        q.pop();

        if (mask == need)
        {
            cout << d[i][j][mask] << endl;
            return 0;
        }

        for (int dir = 0; dir < 4; dir++)
        {
            int ni = i + di[dir];
            int nj = j + dj[dir];
            int nmask = mask;

            if (ni < 0 || ni >= n || nj < 0 || nj >= m)
                continue;

            char c = g[ni][nj];

            if (c == '#')
                continue;

            if (c >= 'A' && c <= 'F')
            {
                int lock = c - 'A';
                if (!(mask & (1 << lock)))
                    continue;
            }

            if (c >= 'a' && c <= 'f')
            {
                int key = c - 'a';
                nmask |= (1 << key);
            }

            if (d[ni][nj][nmask] == -1)
            {
                d[ni][nj][nmask] = d[i][j][mask] + 1;
                q.push({ni, nj, nmask});
            }
        }
    }

    cout << -1 << endl;
    return 0;
}