#include <iostream>
#include <vector>

using namespace std;

void select_sort(vector<int>& vec) {
    int len = vec.size();
    for(int i = 0; i < len; ++i) {
        int minIndex = i;
        for(int j = i + 1; j < len; ++j) {
            if(vec[j] < vec[minIndex]) minIndex = j;
        }
        if(i != minIndex) {
            swap(vec[i], vec[minIndex]);
        }
    }
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

    select_sort(arr);
    cout << "排序后的数组: ";

    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;


    return 0;
}