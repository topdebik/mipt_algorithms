#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> validArrangement(vector<vector<int>> &p)
{
    int n = p.size();
    unordered_map<int, vector<int>> g;
    unordered_map<int, int> in, out;

    for (auto &e : p)
    {
        int u = e[0], v = e[1];
        g[u].push_back(v);
        out[u]++;
        in[v]++;
    }

    int st = p[0][0];
    for (auto &[u, cnt] : out)
    {
        if (cnt == in[u] + 1)
        {
            st = u;
            break;
        }
        else if (cnt > 0)
        {
            st = u;
        }
    }

    vector<int> path;
    stack<int> s;
    s.push(st);

    while (!s.empty())
    {
        int u = s.top();
        if (!g[u].empty())
        {
            int v = g[u].back();
            g[u].pop_back();
            s.push(v);
        }
        else
        {
            path.push_back(u);
            s.pop();
        }
    }

    reverse(path.begin(), path.end());

    vector<vector<int>> res;
    for (int i = 0; i < path.size() - 1; i++)
    {
        res.push_back({path[i], path[i + 1]});
    }

    return res;
}

int main()
{
    vector<vector<int>> pairs = {{5, 1}, {4, 5}, {11, 9}, {9, 4}};
    vector<vector<int>> result = validArrangement(pairs);

    for (auto &p : result)
    {
        cout << "[" << p[0] << "," << p[1] << "] ";
    }
    cout << endl;

    return 0;
}