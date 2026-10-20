#include <cstdio>
#include <cstdlib>
#include "dyn_string.h"

int main() {
    const int CHUNK = 5;  // الدفعة (لا تزيد عن 5)

    std::printf("Enter string: ");
    char* s = read_line_calloc(stdin, CHUNK);
    if (!s) {
        std::puts("Memory not allocated");
        return 1;
    }

    s = process_digits_dyn(s);
    if (!s) {
        std::puts("Memory not allocated");
        return 1;
    }

    std::printf("Result: %s\n", s);
    std::free(s);
    return 0;
}