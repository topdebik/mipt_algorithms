#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// solve1
struct Date
{
    int day, month, year;
};

int dateToDays(const Date &date)
{
    return date.year * 365 + date.month * 31 + date.day;
}

bool compareEvents(const pair<int, int> &a, const pair<int, int> &b)
{
    if (a.first == b.first)
    {
        return a.second < b.second;
    }
    return a.first < b.first;
}

int getMaxContemporaries(const vector<pair<Date, Date>> &people)
{
    vector<pair<int, int>> events;

    for (const auto &person : people)
    {
        const Date &birth = person.first;
        const Date &death = person.second;

        Date start = {birth.day, birth.month, birth.year + 18};
        Date end80 = {birth.day, birth.month, birth.year + 80};
        Date endDeath = death;

        int startDays = dateToDays(start);
        int deathDays = dateToDays(death);
        if (deathDays < startDays)
        {
            continue;
        }

        int endDays80 = dateToDays(end80);
        int endDaysDeath = dateToDays(endDeath);
        int actualEndDays = min(endDays80, endDaysDeath);

        events.push_back({startDays, 1});
        events.push_back({actualEndDays, -1});
    }

    if (events.empty())
    {
        return 0;
    }

    sort(events.begin(), events.end(), compareEvents);

    int maxContemporaries = 0;
    int currentContemporaries = 0;
    for (const auto &event : events)
    {
        currentContemporaries += event.second;
        if (currentContemporaries > maxContemporaries)
        {
            maxContemporaries = currentContemporaries;
        }
    }

    return maxContemporaries;
}

// solve2
vector<int> find_k_smallest(int n, int k)
{
    priority_queue<int> max_heap;
    int num;
    for (int i = 0; i < n; ++i)
    {
        cin >> num;
        if (max_heap.size() < k)
        {
            max_heap.push(num);
        }
        else
        {
            if (num < max_heap.top())
            {
                max_heap.pop();
                max_heap.push(num);
            }
        }
    }
    vector<int> result;
    while (!max_heap.empty())
    {
        result.push_back(max_heap.top());
        max_heap.pop();
    }
    sort(result.begin(), result.end());
    return result;
}

//solve3
int64_t mergeAndCount(vector<int>& arr, vector<int>& temp, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;
    int64_t invCount = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            invCount += (mid - i + 1);
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (i = left; i <= right; ++i) {
        arr[i] = temp[i];
    }

    return invCount;
}

int64_t mergeSortAndCount(vector<int>& arr, vector<int>& temp, int left, int right) {
    int64_t invCount = 0;
    if (left < right) {
        int mid = left + (right - left) / 2;
        invCount += mergeSortAndCount(arr, temp, left, mid);
        invCount += mergeSortAndCount(arr, temp, mid + 1, right);
        invCount += mergeAndCount(arr, temp, left, mid, right);
    }
    return invCount;
}

int64_t countInversions(vector<int>& arr) {
    vector<int> temp(arr.size());
    return mergeSortAndCount(arr, temp, 0, arr.size() - 1);
}

int main()
{
    // solve1
    /*
    int N;
    cin >> N;
    vector<pair<Date, Date>> people;

    for (int i = 0; i < N; ++i)
    {
        Date birth, death;
        cin >> birth.day >> birth.month >> birth.year;
        cin >> death.day >> death.month >> death.year;
        people.push_back({birth, death});
    }

    cout << getMaxContemporaries(people) << endl;
    */

    // solve2
    /*
    int n, k;
    cin >> n >> k;
    vector<int> result = find_k_smallest(n, k);
    for (int num : result)
    {
        cout << num << " ";
    }
    cout << endl;
    */

    //solve3
    vector<int> nums;
    int num;
    while ((cin.peek() != '\n') && (cin >> num)) {
        nums.push_back(num);
    }

    int64_t inversions = countInversions(nums);
    cout << inversions << endl;

    return 0;
}