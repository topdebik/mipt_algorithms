#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int INF = 1e9;

int main()
{
    // Тесткейс: граф с двумя кратчайшими путями
    int n = 4; // вершин
    int m = 4; // рёбер
    int s = 0; // старт (1-я вершина)
    int f = 3; // финиш (4-я вершина)

    vector<vector<int>> g(n);

    // Рёбра графа
    g[0].push_back(1);
    g[1].push_back(0); // 1-2
    g[0].push_back(2);
    g[2].push_back(0); // 1-3
    g[1].push_back(3);
    g[3].push_back(1); // 2-4
    g[2].push_back(3);
    g[3].push_back(2); // 3-4

    vector<int> dist(n, INF);
    vector<int> cnt(n, 0);

    dist[s] = 0;
    cnt[s] = 1;

    queue<int> q;
    q.push(s);

    while (!q.empty())
    {
        int v = q.front();
        q.pop();

        for (int u : g[v])
        {
            if (dist[u] > dist[v] + 1)
            {
                dist[u] = dist[v] + 1;
                cnt[u] = cnt[v];
                q.push(u);
            }
            else if (dist[u] == dist[v] + 1)
            {
                cnt[u] += cnt[v];
            }
        }
    }

    cout << "Количество кратчайших путей: " << cnt[f] << endl;
    cout << "Длина пути: " << dist[f] << endl;

    return 0;
}