#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

const int MAXN = 10010;
vector<int> g[MAXN], gr[MAXN];
bool used[MAXN];
int comp[MAXN];
vector<int> order;

void dfs1(int v)
{
    used[v] = true;
    for (int u : g[v])
    {
        if (!used[u])
        {
            dfs1(u);
        }
    }
    order.push_back(v);
}

void dfs2(int v, int cl)
{
    comp[v] = cl;
    for (int u : gr[v])
    {
        if (comp[u] == -1)
        {
            dfs2(u, cl);
        }
    }
}

int main()
{
    // Тесткейс 1: Граф из двух циклов
    cout << "Тесткейс 1: Два цикла (0-1-2 и 3-4)" << endl;
    int n = 5;
    int m = 4;

    // Очищаем графы
    for (int i = 0; i < n; i++)
    {
        g[i].clear();
        gr[i].clear();
    }

    // Цикл 0->1->2->0
    g[0].push_back(1);
    gr[1].push_back(0);
    g[1].push_back(2);
    gr[2].push_back(1);
    g[2].push_back(0);
    gr[0].push_back(2);

    // Цикл 3->4->3
    g[3].push_back(4);
    gr[4].push_back(3);
    g[4].push_back(3);
    gr[3].push_back(4);

    fill(used, used + n, false);
    order.clear();
    for (int i = 0; i < n; i++)
    {
        if (!used[i])
        {
            dfs1(i);
        }
    }

    fill(comp, comp + n, -1);
    int cnt = 0;
    for (int i = 0; i < n; i++)
    {
        int v = order[n - 1 - i];
        if (comp[v] == -1)
        {
            dfs2(v, cnt++);
        }
    }

    cout << "Найдено компонент: " << cnt << endl;

    if (cnt == 1)
    {
        cout << "Ребра добавлять не нужно: 0" << endl;
    }
    else
    {
        vector<int> in(cnt, 0), out(cnt, 0);
        for (int v = 0; v < n; v++)
        {
            for (int u : g[v])
            {
                if (comp[v] != comp[u])
                {
                    out[comp[v]]++;
                    in[comp[u]]++;
                }
            }
        }

        int src = 0, snk = 0;
        for (int i = 0; i < cnt; i++)
        {
            if (in[i] == 0)
                src++;
            if (out[i] == 0)
                snk++;
        }

        cout << "Источников: " << src << ", Стоков: " << snk << endl;
        cout << "Нужно добавить ребер: " << max(src, snk) << endl;
    }

    cout << endl;

    // Тесткейс 2: Уже сильно связный граф
    cout << "Тесткейс 2: Уже сильно связный граф (0-1-2-0)" << endl;
    n = 3;
    m = 3;

    for (int i = 0; i < n; i++)
    {
        g[i].clear();
        gr[i].clear();
    }

    g[0].push_back(1);
    gr[1].push_back(0);
    g[1].push_back(2);
    gr[2].push_back(1);
    g[2].push_back(0);
    gr[0].push_back(2);

    fill(used, used + n, false);
    order.clear();
    for (int i = 0; i < n; i++)
    {
        if (!used[i])
        {
            dfs1(i);
        }
    }

    fill(comp, comp + n, -1);
    cnt = 0;
    for (int i = 0; i < n; i++)
    {
        int v = order[n - 1 - i];
        if (comp[v] == -1)
        {
            dfs2(v, cnt++);
        }
    }

    cout << "Найдено компонент: " << cnt << endl;
    cout << "Ребра добавлять не нужно: 0" << endl;

    cout << endl;

    // Тесткейс 3: Цепочка компонент
    cout << "Тесткейс 3: Цепочка A->B->C" << endl;
    n = 6;
    m = 5;

    for (int i = 0; i < n; i++)
    {
        g[i].clear();
        gr[i].clear();
    }

    // Компонента A: 0-1 (цикл)
    g[0].push_back(1);
    gr[1].push_back(0);
    g[1].push_back(0);
    gr[0].push_back(1);

    // Компонента B: 2-3 (цикл)
    g[2].push_back(3);
    gr[3].push_back(2);
    g[3].push_back(2);
    gr[2].push_back(3);

    // Компонента C: 4-5 (цикл)
    g[4].push_back(5);
    gr[5].push_back(4);
    g[5].push_back(4);
    gr[4].push_back(5);

    // Связи между компонентами
    g[0].push_back(2); // A->B
    gr[2].push_back(0);
    g[2].push_back(4); // B->C
    gr[4].push_back(2);

    fill(used, used + n, false);
    order.clear();
    for (int i = 0; i < n; i++)
    {
        if (!used[i])
        {
            dfs1(i);
        }
    }

    fill(comp, comp + n, -1);
    cnt = 0;
    for (int i = 0; i < n; i++)
    {
        int v = order[n - 1 - i];
        if (comp[v] == -1)
        {
            dfs2(v, cnt++);
        }
    }

    cout << "Найдено компонент: " << cnt << endl;

    vector<int> in(cnt, 0), out(cnt, 0);
    for (int v = 0; v < n; v++)
    {
        for (int u : g[v])
        {
            if (comp[v] != comp[u])
            {
                out[comp[v]]++;
                in[comp[u]]++;
            }
        }
    }

    int src = 0, snk = 0;
    for (int i = 0; i < cnt; i++)
    {
        if (in[i] == 0)
            src++;
        if (out[i] == 0)
            snk++;
    }

    cout << "Источников: " << src << ", Стоков: " << snk << endl;
    cout << "Нужно добавить ребер: " << max(src, snk) << endl;

    return 0;
}