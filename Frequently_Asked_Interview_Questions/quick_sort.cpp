/**
 * @@@ 快速排序
 * 时间复杂度：
 *  - 平均： O(n logn)
 *  - 最坏： O(n^2) （基本符合预期的时候最差，当划分产生的两个子问题分别包含 n-1 和 0 个元素时，最坏情况发生）
 * 空间复杂度：O(logn) 
 */

#include <iostream>
#include <vector>

using namespace std;

int partition(vector<int>& nums, int low, int high) {
    // int pivot = nums[low]; 普通写法

    // 随机选择pivot，避免最坏情况
    int randomIndex = low + rand() % (high - low + 1);
    swap(nums[low], nums[randomIndex]);
    int pivot = nums[low];
 
    while (low < high) {
        while (low < high && nums[high] >= pivot) --high;
        nums[low] = nums[high];

        while (low < high && nums[low] <= pivot) ++low;
        nums[high] = nums[low];
    }
    nums[low] = pivot;
    return low;
}

void quick_sort(vector<int>& nums, int low, int high) {
    if(low < high) {
        int mid = partition(nums, low, high);
        quick_sort(nums, low, mid - 1);
        quick_sort(nums, mid + 1, high);
    }
}

int main(int argc, const char * argv[]) {
    vector<int> nums = {4, 2, 7, 1, 3, 6};

    cout << "Before sort: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    quick_sort(nums, 0, nums.size() - 1);

    cout << "After sort: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    return 0;
}