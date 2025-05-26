#include <iostream>
#include <vector>

using namespace std;

void insert_sort(vector<int>& vec) {
    int len = vec.size();

    for(int i = 1; i < len; ++i) { // 从第二个元素开始，前面设定为有序
        int key = vec[i]; // 当前需要排序的元素
        int j = i - 1; // 从这里开始往前扫，找到位置，过程中后移
        while(j >= 0 && vec[j] > key) { // 找到第一个比key小的，插在他后面。
            vec[j + 1] = vec[j]; // 后移让位置
            --j;
        }
        vec[j + 1] = key;
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

    insert_sort(arr);
    cout << "排序后的数组: ";

    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;


    return 0;
}