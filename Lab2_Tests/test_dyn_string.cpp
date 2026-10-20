#define _CRT_SECURE_NO_WARNINGS
#include <gtest/gtest.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "dyn_string.h"

namespace {

    // writes text to a temp file and returns the file name
    std::string make_file(const char* text) {
        const std::string name = "dyn_test_tmp.txt";
        FILE* f = std::fopen(name.c_str(), "w");
        std::fputs(text, f);
        std::fclose(f);
        return name;
    }

    std::string read_with_malloc(const char* text) {
        std::string name = make_file(text);
        FILE* f = std::fopen(name.c_str(), "r");
        char* s = read_line_malloc(f);
        std::fclose(f);
        std::remove(name.c_str());
        EXPECT_NE(s, nullptr);
        std::string res(s);
        std::free(s);
        return res;
    }

    std::string read_with_calloc(const char* text, int chunk) {
        std::string name = make_file(text);
        FILE* f = std::fopen(name.c_str(), "r");
        char* s = read_line_calloc(f, chunk);
        std::fclose(f);
        std::remove(name.c_str());
        EXPECT_NE(s, nullptr);
        std::string res(s);
        std::free(s);
        return res;
    }

    std::string process(const char* input) {
        std::string name = make_file(input);
        FILE* f = std::fopen(name.c_str(), "r");
        char* s = read_line_malloc(f);
        std::fclose(f);
        std::remove(name.c_str());
        s = process_digits_dyn(s);
        EXPECT_NE(s, nullptr);
        std::string res(s);
        std::free(s);
        return res;
    }

}  // namespace

// ---- read_line_malloc ----
TEST(ReadMalloc, EmptyLine) {
    EXPECT_EQ(read_with_malloc("\n"), "");
}

TEST(ReadMalloc, SingleChar) {
    EXPECT_EQ(read_with_malloc("a\n"), "a");
}

TEST(ReadMalloc, LongLine) {
    EXPECT_EQ(read_with_malloc("hello world 123\n"), "hello world 123");
}

TEST(ReadMalloc, NoTrailingNewline) {
    EXPECT_EQ(read_with_malloc("abc"), "abc");
}

// ---- read_line_calloc ----
TEST(ReadCalloc, EmptyLine) {
    EXPECT_EQ(read_with_calloc("\n", 5), "");
}

TEST(ReadCalloc, ShorterThanChunk) {
    EXPECT_EQ(read_with_calloc("abc\n", 5), "abc");
}

TEST(ReadCalloc, ExactlyChunk) {
    EXPECT_EQ(read_with_calloc("abcde\n", 5), "abcde");
}

TEST(ReadCalloc, LongerThanChunk) {
    EXPECT_EQ(read_with_calloc("abcdefghijklmnop\n", 5), "abcdefghijklmnop");
}

TEST(ReadCalloc, ChunkOfOne) {
    EXPECT_EQ(read_with_calloc("xyz123\n", 1), "xyz123");
}

// ---- process_digits_dyn ----
TEST(ProcessDyn, EmptyString) {
    EXPECT_EQ(process("\n"), "");
}

TEST(ProcessDyn, NoDigits) {
    EXPECT_EQ(process("abc\n"), "abc");
}

TEST(ProcessDyn, AllOdd) {
    EXPECT_EQ(process("13579\n"), "");
}

TEST(ProcessDyn, AllEven) {
    EXPECT_EQ(process("2468\n"), "2++4++6++8");
}

TEST(ProcessDyn, EvenDigitAtEnd) {
    EXPECT_EQ(process("x8\n"), "x8");
}

TEST(ProcessDyn, EvenDigitNotAtEnd) {
    EXPECT_EQ(process("x2y\n"), "x2++y");
}

TEST(ProcessDyn, FullExample) {
    EXPECT_EQ(process("a1b2c3d4 55 x8\n"), "ab2++cd4++  x8");
}