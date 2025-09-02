#include <iostream>
#include <vector>
#include <sstream>
#include <stack>

using namespace std;

// Given n non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it can trap after raining.
void solve1()
{
    std::vector<int> height = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    int left = 0, right = height.size() - 1;
    int left_max = 0, right_max = 0;
    int total_water = 0;

    while (left < right)
    {
        if (height[left] <= height[right])
        {
            if (height[left] >= left_max)
            {
                left_max = height[left];
            }
            else
            {
                total_water += left_max - height[left];
            }
            left++;
        }
        else
        {
            if (height[right] >= right_max)
            {
                right_max = height[right];
            }
            else
            {
                total_water += right_max - height[right];
            }
            right--;
        }
    }

    std::cout << total_water << std::endl;
}

// You are given an absolute path for a Unix-style file system, which always begins with a slash '/'. Your task is to transform this absolute path into its simplified canonical path. The rules of a Unix-style file system are as follows: A single period '.' represents the current directory. A double period '..' represents the previous/parent directory. Multiple consecutive slashes such as '//' and '///' are treated as a single slash '/'. Any sequence of periods that does not match the rules above should be treated as a valid directory or file name. For example, '...' and '....' are valid directory or file names. The simplified canonical path should follow these rules:The path must start with a single slash '/'. Directories within the path must be separated by exactly one slash '/'. The path must not end with a slash '/', unless it is the root directory. The path must not have any single or double periods ('.' and '..') used to denote current or parent directories. Return the simplified canonical path.
void solve2()
{
    string path = "/.../a/../b/c/../d/./";

    vector<string> parts;
    stringstream ss(path);
    string item;

    while (getline(ss, item, '/'))
    {
        if (item == "" || item == ".")
        {
            continue;
        }
        else if (item == "..")
        {
            if (!parts.empty())
            {
                parts.pop_back();
            }
        }
        else
        {
            parts.push_back(item);
        }
    }

    string simplified_path = "/";
    for (const string &part : parts)
    {
        simplified_path += part + "/";
    }

    if (simplified_path.length() > 1)
    {
        simplified_path.pop_back();
    }

    cout << simplified_path << endl;
}

// Given an array of integers heights representing the histogram's bar height where the width of each bar is 1, return the area of the largest rectangle in the histogram.
void solve3()
{
    vector<int> heights = {2, 1, 5, 6, 2, 3};
    stack<int> indices;
    int max_area = 0;
    int current_index = 0;

    while (current_index < heights.size())
    {
        if (indices.empty() || heights[current_index] >= heights[indices.top()])
        {
            indices.push(current_index++);
        }
        else
        {
            int top_index = indices.top();
            indices.pop();
            int height = heights[top_index];
            int width = indices.empty() ? current_index : current_index - indices.top() - 1;
            max_area = max(max_area, height * width);
        }
    }

    while (!indices.empty())
    {
        int top_index = indices.top();
        indices.pop();
        int height = heights[top_index];
        int width = indices.empty() ? current_index : current_index - indices.top() - 1;
        max_area = max(max_area, height * width);
    }

    cout << max_area << endl;
}

// Definition for singly-linked list
struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Print singly-linked list
void print_list(ListNode *head)
{
    ListNode *current = head;
    while (current != nullptr)
    {
        std::cout << current->val;
        if (current->next != nullptr)
        {
            std::cout << " ";
        }
        current = current->next;
    }
    std::cout << std::endl;
}

// Given the head of a sorted linked list, delete all nodes that have duplicate numbers, leaving only distinct numbers from the original list. Return the linked list sorted as well.
ListNode *solve4(ListNode *head)
{
    ListNode dummy(0);
    ListNode *prev = &dummy;
    dummy.next = head;

    while (head)
    {
        if (head->next && head->val == head->next->val)
        {
            while (head->next && head->val == head->next->val)
            {
                head = head->next;
            }
            prev->next = head->next;
        }
        else
        {
            prev = prev->next;
        }
        head = head->next;
    }

    return dummy.next;
}

// You are given the heads of two sorted linked lists list1 and list2. Merge the two lists into one sorted list. The list should be made by splicing together the nodes of the first two lists. Return the head of the merged linked list.
ListNode *solve5(ListNode *list1, ListNode *list2)
{
    ListNode dummy(0);
    ListNode *current = &dummy;

    while (list1 && list2)
    {
        if (list1->val < list2->val)
        {
            current->next = list1;
            list1 = list1->next;
        }
        else
        {
            current->next = list2;
            list2 = list2->next;
        }
        current = current->next;
    }

    current->next = list1 ? list1 : list2;

    return dummy.next;
}

int main()
{
    solve1();

    solve2();

    solve3();

    // [1,1,1,2,3]
    ListNode *_4 = new ListNode(3);
    ListNode *_3 = new ListNode(2, _4);
    ListNode *_2 = new ListNode(1, _3);
    ListNode *_1 = new ListNode(1, _2);
    ListNode *_0 = new ListNode(1, _1);
    _0 = solve4(_0);
    print_list(_0);

    // [1,2,4], [1,3,4]
    ListNode *_1_2 = new ListNode(4);
    ListNode *_1_1 = new ListNode(2, _1_2);
    ListNode *_1_0 = new ListNode(1, _1_1);
    ListNode *_2_2 = new ListNode(4);
    ListNode *_2_1 = new ListNode(3, _2_2);
    ListNode *_2_0 = new ListNode(1, _2_1);
    _0 = solve5(_1_0, _2_0);
    print_list(_0);
    return 0;
}
