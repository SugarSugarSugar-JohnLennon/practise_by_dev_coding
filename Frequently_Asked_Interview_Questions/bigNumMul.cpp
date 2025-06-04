#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

string bitMul(string str, char c) {
    reverse(str.begin(), str.end());
    int carry = 0, bitRes = 0, i = 0;
    string res;
    
    while(i < str.size() || carry != 0) {
        int md = c - '0';
        int mn = i < str.size() ? str[i] - '0' : 0;
        i++;
        bitRes = (carry + md * mn) % 10;
        carry = (carry + md * mn) / 10;
        res.push_back(bitRes + '0');
    }
    reverse(res.begin(), res.end());
    return res;
}

string strAdd(string str1, string str2) {
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

string strMul(string str1, string str2) {
    vector<string> lst;
    
    reverse(str1.begin(), str1.end());
    for(int i = 0; i < str1.size(); ++i) {
        string tmp = bitMul(str2, str1[i]);
        for(int j = 0; j < i; ++j) {
            tmp.push_back('0');
        }
        lst.push_back(tmp);
    }
    
    string res;
    for(auto r : lst) {
        res = strAdd(res, r);
    }
    return res;
}

int main() {
   string a = "123";
   string b = "456";
   cout << "123 * 456 = " << strMul(a, b) << endl;
   return 0;
}
