/*
 * 用vim写代码感受下。写个稍微要点时间的
 * 写起来还可以，还是挺给力的
 * 大数相加
 * 也是比较熟悉了
 * vim写起来没太大困难
 * @Date 2025-07-01-18:27:00
 *
 */

#include <iostream>
#include <algorithm>

using namespace std;


string add(string s1, string s2) {
    reverse(s1.begin(), s1.end());
    reverse(s2.begin(), s2.end());
    
    int sum = 0;
    int carry = 0;
    int index = 0;
    string res;
    while(index < s1.size() || index < s2.size() || carry != 0) {
        int first = index > s1.size() ? 0 : (s1[index] - '0');
        int second = index > s2.size() ? 0 : (s2[index] - '0');
        
        sum = (first + second + carry) % 10;
        carry = sum / 10;

        res.push_back(sum + '0');

        ++index;
    }
    reverse(res.begin(), res.end());

    return res;
}

int main() {
    string a = "123", b = "456";
    cout << a << " + " << b << " = " << add(a, b) << endl;
    return 0;
}
