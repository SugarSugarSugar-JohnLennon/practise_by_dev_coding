#include <iostream>
#include <vector>

using namespace std;

int partition(vector<int>& nums, int low, int high) {
    int pivot = nums[high];
    int i = low - 1;
    
    for(int j = low; j < high; ++j) {
        if(nums[j] <= pivot) {
            ++i;
            swap(nums[j], nums[i]);
        }
    }
    swap(nums[i + 1], nums[high]);
    return i + 1;
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