#include <iostream>
#include <vector>

using namespace std;

// 合并[left,mid] ,[mid + 1, right]
void merge(vector<int>& vec, int left, int mid, int right) {
    vector<int> tmp;
    int i = left, j = mid + 1;

    // 合并两个子数组
    while(i <= mid && j <= right) {
        if(vec[i] < vec[j]) tmp.push_back(vec[i++]);
        else tmp.push_back(vec[j++]);
    }

    // 处理剩余部分
    while (i <= mid) tmp.push_back(vec[i++]);
    while (j <= right) tmp.push_back(vec[j++]);

    // 拷贝回原数组
    for (int k = 0; k < tmp.size(); ++k) {
        vec[left + k] = tmp[k];
    }
}

void merge_sort(vector<int>& vec, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    merge_sort(vec, left, mid);      // 排左边
    merge_sort(vec, mid + 1, right); // 排右边
    merge(vec, left, mid, right);   // 合并
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

    merge_sort(arr, 0, arr.size() - 1);
    cout << "排序后的数组: ";

    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;


    return 0;
}