#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// solve1
void mergeInPlace(vector<int> &arr, int left, int mid, int right)
{
    int i = left;
    int j = mid + 1;

    if (arr[mid] <= arr[j])
    {
        return;
    }

    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
        {
            i++;
        }
        else
        {
            int value = arr[j];
            int index = j;

            while (index > i)
            {
                arr[index] = arr[index - 1];
                index--;
            }

            arr[i] = value;

            i++;
            mid++;
            j++;
        }
    }
}

void mergeSortInPlace(vector<int> &arr)
{
    int n = arr.size();

    for (int curr_size = 1; curr_size < n; curr_size *= 2)
    {
        for (int left = 0; left < n - 1; left += 2 * curr_size)
        {
            int mid = left + curr_size - 1;
            int right = min(left + 2 * curr_size - 1, n - 1);

            mergeInPlace(arr, left, mid, right);
        }
    }
}

void printArray(const vector<int> &arr)
{
    for (int num : arr)
        cout << num << " ";
    cout << endl;
}

// solve2
struct Point
{
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

bool comparePoints(const Point &a, const Point &b)
{
    if (a.x != b.x)
    {
        return a.x < b.x;
    }
    else
    {
        return a.y < b.y;
    }
}

vector<Point> buildPolyline(vector<Point> points)
{
    sort(points.begin(), points.end(), comparePoints);

    return points;
}

// solve3
vector<int> read_priority(int k)
{
    vector<int> priority(k);
    for (int i = 0; i < k; ++i)
    {
        cin >> priority[i];
        priority[i]--;
    }
    return priority;
}

vector<vector<string>> read_records(int N, int k)
{
    vector<vector<string>> records(N, vector<string>(k + 1));
    for (int i = 0; i < N; ++i)
    {
        cin >> records[i][0];
        for (int j = 1; j <= k; ++j)
        {
            cin >> records[i][j];
        }
    }
    return records;
}

bool compare_records(const vector<string> &a, const vector<string> &b, const vector<int> &priority)
{
    for (int p : priority)
    {
        if (a[p + 1] != b[p + 1])
        {
            return a[p + 1] > b[p + 1];
        }
    }
    return false;
}

void sort_records(vector<vector<string>> &records, const vector<int> &priority)
{
    auto comparator = [&priority](const vector<string> &a, const vector<string> &b)
    {
        return compare_records(a, b, priority);
    };
    sort(records.begin(), records.end(), comparator);
}

void print_records(const vector<vector<string>> &records)
{
    for (const auto &record : records)
    {
        cout << record[0] << endl;
    }
}

int main()
{
    // solve1
    /*
    vector<int> arr = {12, 11, 13, 5, 6, 7, 1, 8};
    printArray(arr);
    mergeSortInPlace(arr);
    printArray(arr);
    */

    // solve2
    /*
    int N;
    cin >> N;
    vector<Point> points(N);
    for (int i = 0; i < N; ++i)
    {
        cin >> points[i].x >> points[i].y;
    }
    vector<Point> polyline = buildPolyline(points);
    for (const Point &p : polyline)
    {
        cout << p.x << " " << p.y << endl;
    }
    */

    // solve3
    int n, k;
    cin >> n >> k;
    vector<int> priority = read_priority(k);
    vector<vector<string>> records = read_records(n, k);
    sort_records(records, priority);
    print_records(records);

    return 0;
}