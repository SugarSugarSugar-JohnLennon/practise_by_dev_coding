#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

namespace StringUtils {
    // 字符串操作
    std::string toUpperCase(const std::string& str);
    std::string toLowerCase(const std::string& str);
    std::string reverse(const std::string& str);
    
    // 字符串分割和连接
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string join(const std::vector<std::string>& strings, const std::string& separator);
    
    // 字符串检查
    bool startsWith(const std::string& str, const std::string& prefix);
    bool endsWith(const std::string& str, const std::string& suffix);
    bool contains(const std::string& str, const std::string& substring);
}

#endif // STRING_UTILS_H 