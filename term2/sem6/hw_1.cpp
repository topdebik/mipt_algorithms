#include <bits/stdc++.h>
using namespace std;

vector<string> findItinerary(vector<vector<string>> &t)
{
    unordered_map<string, priority_queue<string, vector<string>, greater<string>>> g;

    for (auto &ticket : t)
    {
        g[ticket[0]].push(ticket[1]);
    }

    vector<string> res;

    function<void(string)> dfs = [&](string u)
    {
        auto &pq = g[u];
        while (!pq.empty())
        {
            string v = pq.top();
            pq.pop();
            dfs(v);
        }
        res.push_back(u);
    };

    dfs("JFK");
    reverse(res.begin(), res.end());
    return res;
}

int main()
{
    vector<vector<string>> t = {{"MUC", "LHR"}, {"JFK", "MUC"}, {"SFO", "SJC"}, {"LHR", "SFO"}};
    vector<string> r = findItinerary(t);

    for (auto &s : r)
        cout << s << " ";
    cout << endl;

    return 0;
}
