#include <iostream>
#include <vector>

using namespace std;

int partition(vector<int>& nums, int low, int high) {
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