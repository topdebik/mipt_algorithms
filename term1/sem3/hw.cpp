#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>

using namespace std;

// Given two sorted arrays nums1 and nums2 of size m and n respectively, return the median of the two sorted arrays
float solve1(vector<int> &nums1, vector<int> &nums2)
{
    if (nums1.size() > nums2.size())
    {
        return solve1(nums2, nums1);
    }

    int m = nums1.size();
    int n = nums2.size();
    int low = 0, high = m;

    while (low <= high)
    {
        int part1 = (low + high) / 2;
        int part2 = (m + n + 1) / 2 - part1;

        int maxLeft1 = (part1 == 0) ? INT_MIN : nums1[part1 - 1];
        int minRight1 = (part1 == m) ? INT_MAX : nums1[part1];

        int maxLeft2 = (part2 == 0) ? INT_MIN : nums2[part2 - 1];
        int minRight2 = (part2 == n) ? INT_MAX : nums2[part2];

        if (maxLeft1 <= minRight2 && maxLeft2 <= minRight1)
        {
            if ((m + n) % 2 == 0)
            {
                return (max(maxLeft1, maxLeft2) + min(minRight1, minRight2)) / 2.0;
            }
            else
            {
                return max(maxLeft1, maxLeft2);
            }
        }
        else if (maxLeft1 > minRight2)
        {
            high = part1 - 1;
        }
        else
        {
            low = part1 + 1;
        }
    }

    throw invalid_argument("Input arrays are not sorted.");
}

bool canReach(vector<int> &a, int maxJump, int n, int k) 
{
    if (a[0] == 0) return false;
    int jumps = 0;
    int currentPosition = 0;

    while (currentPosition < n) 
    {
        if (currentPosition + maxJump >= n - 1) return true;
        int nextPosition = currentPosition;

        for (int jump = 1; jump <= maxJump; ++jump) 
        {
            if (currentPosition + jump < n && a[currentPosition + jump] == 1) 
            {
                nextPosition = currentPosition + jump;
            }
        }
        if (nextPosition == currentPosition) 
        {
            return false;
        }
        currentPosition = nextPosition;
        jumps++;
        if (jumps > k) 
        {
            return false;
        }
    }
    return true;

}

// The frog is located at a point with coordinate 0 and jumps along the coordinate axis (in the direction of increase) by integer distances. It wants to get to a point with coordinate n in no more than k jumps. Some points are forbidden for landing: if a[i] = 1, then the point with coordinate i is allowed for landing, and if a[i] = 0, it is forbidden (i = 1...n – 1). In addition, the frog tries to avoid long jumps, i.e. it is required to find a route that would minimize the length of the longest jump. The input data contains natural numbers n and k in the first line (2 ≤ n ≤ 105) and a string of n − 1 symbols a[i] (each symbol is zero or one) in the second. It is required to output the length of the longest jump in the sought route. The required complexity is O(nlog(n)). The input data contains natural numbers n and k in the first line (2 ≤ n ≤ 105) and a string of n − 1 symbols a[i] (each symbol is zero or one) in the second. The output is required to be the length of the longest jump in the desired route.
int solve2(vector<int> &n_k, vector<int> &a)
{
    int n = n_k[0];
    int k = n_k[1];

    int left = 1, right = n;
    int result = n;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (canReach(a, mid, n, k))
        {
            result = mid;
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    return result;
}

// An array of integers A[0...n) is given. The array is arbitrarily filled with natural numbers from the range [0…n - 1). One or more values ​​in the array may be repeated. It is necessary to find any repetition in O(n), memory O(1). The original array can be stored, but cannot be modified. n ≤ 10000.
int solve3(vector<int> &A)
{
    for (int i = 0; i < A.size(); i++)
    {
        int index = abs(A[i]);
        if (A[index] < 0)
        {
            return index;
        }
        A[index] = -A[index];
    }
    return -1;
}

// Given a sorted array of integers A[0..n-1] and an array of integers B[0..m-1]. For each element of array B[i], find the minimum index k of the minimum element of array A equal to or greater than B[i]: A[k] >= B[i]. If there is no such element, print n. The running time of finding k for each element of B[i] is O(log(k)). n, m ≤ 10000.
vector<int> solve4(vector<int> &A, vector<int> &B)
{
    int n = A.size();
    vector<int> result;

    for (int b : B)
    {
        int low = 0, high = n;
        while (low < high)
        {
            int mid = (low + high) / 2;
            if (A[mid] < b)
            {
                low = mid + 1;
            }
            else
            {
                high = mid;
            }
        }
        result.push_back((low != high) ? low : high);
    }

    return result;
}

int main()
{
    vector<int> a{1, 2};
    vector<int> b{3, 4};
    cout << solve1(a, b) << endl;

    // NOT WORKING
    a = {10, 2};
    b = {0, 0, 1, 0, 0, 1, 0, 0, 0};
    cout << solve2(a, b) << endl;

    a = {1, 2, 4, 5, 6, 1, 0, 3};
    cout << solve3(a) << endl;

    a = {2, 4, 5, 7};
    b = {4, 6, 1};
    for (int i : solve4(a, b))
    {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
