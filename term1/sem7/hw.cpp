#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void solve1()
{
    int n;
    cin >> n;

    priority_queue<long long, vector<long long>, greater<long long>> min_heap;

    for (int i = 0; i < n; ++i)
    {
        long long num;
        cin >> num;
        min_heap.push(num);
    }

    long long total_time = 0;

    while (min_heap.size() > 1)
    {
        long long a = min_heap.top();
        min_heap.pop();
        long long b = min_heap.top();
        min_heap.pop();

        long long sum = a + b;
        total_time += sum;

        min_heap.push(sum);
    }

    cout << total_time << endl;
}

struct Train
{
    int arrival;
    int departure;
};

void solve2()
{
    int n;
    cin >> n;

    vector<Train> trains(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> trains[i].arrival >> trains[i].departure;
    }

    priority_queue<int, vector<int>, greater<int>> freeTracks;

    for (const auto &train : trains)
    {
        if (!freeTracks.empty() && freeTracks.top() < train.arrival)
        {
            freeTracks.pop();
        }
        freeTracks.push(train.departure);
    }

    cout << freeTracks.size() << endl;
}

vector<int> slidingWindowMax(const vector<int> &nums, int k)
{
    vector<int> result;
    priority_queue<pair<int, int>> heap;

    for (int i = 0; i < nums.size(); ++i)
    {
        heap.push({nums[i], i});

        if (i >= k - 1)
        {
            while (heap.top().second <= i - k)
            {
                heap.pop();
            }
            result.push_back(heap.top().first);
        }
    }

    return result;
}

void solve3()
{
    int n;
    cin >> n;

    vector<int> A(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> A[i];
    }

    int k;
    cin >> k;

    vector<int> result = slidingWindowMax(A, k);

    for (int num : result)
    {
        cout << num << " ";
    }
    cout << endl;
}

int main()
{
    // solve1();
    // solve2();
    solve3();

    return 0;
}