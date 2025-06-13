#ifndef _NONCOPYABLE_H_
#define _NONCOPYABLE_H_

namespace sugar {

class NonCopyable {
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

} // namespace sugar

#endif // _NONCOPYABLE_H_