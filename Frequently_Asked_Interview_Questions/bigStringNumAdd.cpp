#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string strAdd(string& str1, string& str2) {
    reverse(str1.begin(), str1.end());
    reverse(str2.begin(), str2.end());
    string res;
    int i = 0, sum = 0, carry = 0;
    while(i < str1.size() || i < str2.size() || carry != 0) {
        int s1 = i < str1.size() ? str1[i] - '0' : 0;
        int s2 = i < str2.size() ? str2[i] - '0' : 0;
        sum = (s1 + s2 + carry) % 10;
        carry = (s1 + s2 + carry) / 10;
        res.push_back(sum + '0');
        i++;
    }
    reverse(res.begin(), res.end());
    return res;
}

int main() {
    string a = "123456789";
    string b = "987654321";
    cout << "a + b = " << strAdd(a, b) << endl;
    string c = "999999999999999999999999";
    string d = "1";
    cout << "c + d = " << strAdd(c, d) << endl;
    return 0;
}