#include "dyn_string.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace {
    bool isDigit(char c) { return c >= '0' && c <= '9'; }
    bool isEven(char c) { return (c - '0') % 2 == 0; }
}

char* read_line_malloc(std::FILE* in) {
    // بلوك أولي لحرف واحد (للـ '\0')
    char* buf = static_cast<char*>(std::malloc(1));
    if (!buf) return nullptr;
    buf[0] = '\0';
    std::size_t len = 0;

    int c;
    while ((c = std::fgetc(in)) != EOF && c != '\n') {
        // نحجز بلوك جديد أكبر بحرف واحد
        char* bigger = static_cast<char*>(std::malloc(len + 2));
        if (!bigger) {
            std::free(buf);
            return nullptr;
        }
        std::memcpy(bigger, buf, len);   // ننسخ القديم
        bigger[len] = static_cast<char>(c);
        bigger[len + 1] = '\0';
        std::free(buf);                  // نحرر القديم
        buf = bigger;
        ++len;
    }
    return buf;
}

char* read_line_calloc(std::FILE* in, int chunk) {
    if (chunk < 1) chunk = 1;
    if (chunk > 5) chunk = 5;

    std::size_t cap = static_cast<std::size_t>(chunk) + 1;  // + '\0'
    char* buf = static_cast<char*>(std::calloc(cap, sizeof(char)));
    if (!buf) return nullptr;
    std::size_t len = 0;

    int c;
    while ((c = std::fgetc(in)) != EOF && c != '\n') {
        if (len + 1 >= cap) {  // ما بقي مكان للحرف + '\0'
            std::size_t newCap = cap + static_cast<std::size_t>(chunk);
            char* bigger = static_cast<char*>(std::realloc(buf, newCap));
            if (!bigger) {
                std::free(buf);
                return nullptr;
            }
            buf = bigger;
            cap = newCap;
        }
        buf[len++] = static_cast<char>(c);
        buf[len] = '\0';
    }
    return buf;
}

char* process_digits_dyn(char* s) {
    if (!s) return nullptr;
    std::size_t len = std::strlen(s);
    if (len == 0) return s;
    bool lastEven = isDigit(s[len - 1]) && isEven(s[len - 1]);

    // 1) حذف الأرقام الفردية
    std::size_t w = 0;
    for (std::size_t r = 0; r < len; ++r) {
        if (isDigit(s[r]) && !isEven(s[r])) continue;
        s[w++] = s[r];
    }
    s[w] = '\0';
    len = w;

    // 2) عدّ الأرقام (كلها زوجية الآن)
    std::size_t cnt = 0;
    for (std::size_t i = 0; i < len; ++i)
        if (isDigit(s[i])) ++cnt;
    if (lastEven && cnt > 0) --cnt;

    std::size_t newLen = len + 2 * cnt;
    if (newLen == len) return s;  // ما في شي نضيفه

    // 3) نكبّر البلوك بـ realloc
    char* t = static_cast<char*>(std::realloc(s, newLen + 1));
    if (!t) {
        std::free(s);
        return nullptr;
    }
    s = t;

    // 4) التوسيع من النهاية للبداية
    s[newLen] = '\0';
    std::ptrdiff_t r = static_cast<std::ptrdiff_t>(len) - 1;
    std::ptrdiff_t p = static_cast<std::ptrdiff_t>(newLen) - 1;
    while (r >= 0) {
        if (isDigit(s[r]) && !(lastEven && r == static_cast<std::ptrdiff_t>(len) - 1)) {
            s[p--] = '+';
            s[p--] = '+';
        }
        s[p--] = s[r];
        --r;
    }
    return s;
}