#include <iostream>
#include <vector>

using namespace std;

int parition(vector<int>& vec, int low, int high) {
    int pivot = vec[high]; // 基准值
    int i = low - 1;       // 小于基准的

    for(int j = low; j < high; ++j) {
        if(vec[j] <= pivot){
            ++i;
            swap(vec[i], vec[j]);
        }
    }

    swap(vec[i + 1], vec[high]);
    return i + 1;
}

void quick_sort_helper(vector<int>& vec, int low, int high) {
    if (low < high) {
        int pi = parition(vec, low, high); // 分区
        quick_sort_helper(vec, low, pi - 1);      // 排左半边
        quick_sort_helper(vec, pi + 1, high);     // 排右半边
    }
}

void quick_sort(vector<int>& vec) {
     quick_sort_helper(vec, 0, vec.size() - 1);
}

int main(){
    int n;
    cout << "请输入元素个数: ";
    cin >> n;

    vector<int> arr(n);
    cout << "请输入 " << n << " 个整数: ";
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    quick_sort(arr);
    cout << "排序后的数组: ";

    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;


    return 0;
}