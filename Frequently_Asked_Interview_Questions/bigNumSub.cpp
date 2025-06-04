#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string strSub(string& str1, string& str2) {
    bool negative = false;
    // 假设 str1 >= str2
    if(str1 < str2) {
        swap(str1, str2);
        negative = true;
    }
    
    reverse(str1.begin(), str1.end());
    reverse(str2.begin(), str2.end());
    string res;
    int i = 0, diff = 0, borrow = 0;
    while (i < str1.size()) {
        int s1 = str1[i] - '0';
        int s2 = i < str2.size() ? str2[i] - '0' : 0;

        diff = s1 - s2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        res.push_back(diff + '0');
        i++;
    }

    // 去除前导零
    while (res.size() > 1 && res.back() == '0') {
        res.pop_back();
    }
    
    if(negative) res.push_back('-');
    reverse(res.begin(), res.end());
    return res;
}

int main() {
    string b = "987654321";
    string a = "123456789";
    cout << "a - b = " << strSub(a, b) << endl;

    string c = "100000000000000000000000";
    string d = "1";
    cout << "c - d = " << strSub(c, d) << endl;

    return 0;
}
