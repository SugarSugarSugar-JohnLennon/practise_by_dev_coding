#include <iostream>
#include <vector>

using namespace std;

void bubble_sort(vector<int>& vec) {
    int len = vec.size();
    bool swapped;

    for(int i = 0; i < len; ++i) {
        swapped = false;
        for(int j = 0; j < len - (i + 1); ++j) {
            if(vec[j] > vec[j + 1]) {
                swap(vec[j], vec[j + 1]);
                swapped = true;
            }
        }
        if(!swapped) break;
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

    bubble_sort(arr);
    cout << "排序后的数组: ";

    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;


    return 0;
}