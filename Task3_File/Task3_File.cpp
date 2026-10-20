#include <cstdio>
#include <cstdlib>
#include "dyn_string.h"

int main() {
    // 1) إدخال السلسلة بطريقة malloc وكتابتها بالملف input.txt
    std::printf("Enter string: ");
    char* s = read_line_malloc(stdin);
    if (!s) {
        std::puts("Memory not allocated");
        return 1;
    }

    FILE* out1 = std::fopen("input.txt", "w");
    if (!out1) {
        std::puts("Cannot open input.txt");
        std::free(s);
        return 1;
    }
    std::fputs(s, out1);
    std::fputs("\n", out1);
    std::fclose(out1);
    std::free(s);

    // 2) قراءة السلسلة من الملف (بنفس طريقة malloc) ومعالجتها
    FILE* in = std::fopen("input.txt", "r");
    if (!in) {
        std::puts("Cannot open input.txt");
        return 1;
    }
    char* t = read_line_malloc(in);
    std::fclose(in);
    if (!t) {
        std::puts("Memory not allocated");
        return 1;
    }

    t = process_digits_dyn(t);
    if (!t) {
        std::puts("Memory not allocated");
        return 1;
    }

    // 3) كتابة النتيجة بـ output.txt
    FILE* out2 = std::fopen("output.txt", "w");
    if (!out2) {
        std::puts("Cannot open output.txt");
        std::free(t);
        return 1;
    }
    std::fputs(t, out2);
    std::fputs("\n", out2);
    std::fclose(out2);
    std::free(t);

    std::puts("Done. Result written to output.txt");
    return 0;
}