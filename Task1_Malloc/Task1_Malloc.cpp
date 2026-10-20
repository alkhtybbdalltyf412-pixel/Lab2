#include <cstdio>
#include <cstdlib>
#include "dyn_string.h"

int main() {
    std::printf("Enter string: ");
    char* s = read_line_malloc(stdin);
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