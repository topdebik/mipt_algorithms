#include <vector>
#include <iostream>
using namespace std;

void dfs(vector<vector<int>> &g, vector<bool> &vis, int u)
{
    vis[u] = true;
    for (int v = 0; v < g.size(); v++)
    {
        if (g[u][v] == 1 && !vis[v])
        {
            dfs(g, vis, v);
        }
    }
}

int findCircleNum(vector<vector<int>> &g)
{
    int n = g.size();
    vector<bool> vis(n, false);
    int cnt = 0;

    for (int i = 0; i < n; i++)
    {
        if (!vis[i])
        {
            cnt++;
            dfs(g, vis, i);
        }
    }

    return cnt;
}

int main()
{
    vector<vector<int>> isConnected = {{1, 1, 0}, {1, 1, 0}, {0, 0, 1}};

    int result = findCircleNum(isConnected);

    cout << result << endl;

    return 0;
}