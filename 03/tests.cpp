#include "parser.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(Parse, SimpleTokens) {
    std::vector<uint64_t> digits;
    std::vector<std::string> strings;

    auto digit_ptr = [&digits](uint64_t digit) {
        digits.push_back(digit);
    };

    auto str_ptr = [&strings](const std::string& str) {
        strings.push_back(str);
    };

    std::string text = "test 42 \ncode2 007";
    parse(text, digit_ptr, str_ptr);

    std::vector<uint64_t> digits_expected = {42, 7};
    std::vector<std::string> strings_expected = {"test", "code2"};

    ASSERT_EQ(digits, digits_expected);
    ASSERT_EQ(strings, strings_expected);
}

TEST(Parse, NoDigitCallback) {
    std::vector<std::string> strings;

    auto str_ptr = [&strings](const std::string& str) {
        strings.push_back(str);
    };

    std::string text = "sample 25 \ndata3 005 77 000 24\n19";
    parse(text, nullptr, str_ptr);

    std::vector<std::string> strings_expected = {"sample", "data3"};

    ASSERT_EQ(strings, strings_expected);
}

TEST(Parse, NoStringCallback) {
    std::vector<uint64_t> digits;

    auto digit_ptr = [&digits](uint64_t digit) {
        digits.push_back(digit);
    };

    std::string text = "text 33 \nvalue4 003 88 000 27\n15";
    parse(text, digit_ptr, nullptr);

    std::vector<uint64_t> digits_expected = {33, 3, 88, 0, 27, 15};

    ASSERT_EQ(digits, digits_expected);
}

TEST(Parse, LargeNumbers) {
    std::vector<uint64_t> digits;
    std::vector<std::string> strings;

    auto digit_ptr = [&digits](uint64_t digit) {
        digits.push_back(digit);
    };

    auto str_ptr = [&strings](const std::string& str) {
        strings.push_back(str);
    };

    std::string text = "18446744073709551613  \n 18446744073709551615 18446744073709551617 000018446744073709551615";
    parse(text, digit_ptr, str_ptr);

    std::vector<uint64_t> digits_expected = {UINT64_MAX - 2, UINT64_MAX, UINT64_MAX};
    std::vector<std::string> strings_expected = {"18446744073709551617"};

    ASSERT_EQ(digits, digits_expected);
    ASSERT_EQ(strings, strings_expected);
}

TEST(Parse, EmptyAndWhitespace) {
    std::vector<uint64_t> digits;
    std::vector<std::string> strings;

    auto digit_ptr = [&digits](uint64_t digit) {
        digits.push_back(digit);
    };

    auto str_ptr = [&strings](const std::string& str) {
        strings.push_back(str);
    };

    std::string text = "\t \n  ";
    parse(text, digit_ptr, str_ptr);

    std::vector<uint64_t> digits_expected;
    std::vector<std::string> strings_expected;

    ASSERT_EQ(digits, digits_expected);
    ASSERT_EQ(strings, strings_expected);

    text = "";
    parse(text, digit_ptr, str_ptr);

    ASSERT_EQ(digits, digits_expected);
    ASSERT_EQ(strings, strings_expected);
}

TEST(Parse, ZeroPadding) {
    std::vector<uint64_t> digits;
    std::vector<std::string> strings;

    auto digit_ptr = [&digits](uint64_t digit) {
        digits.push_back(digit);
    };

    auto str_ptr = [&strings](const std::string& str) {
        strings.push_back(str);
    };

    std::string text = "0000 00123 0000001 0";
    parse(text, digit_ptr, str_ptr);

    std::vector<uint64_t> digits_expected = {0, 123, 1, 0};
    std::vector<std::string> strings_expected;

    ASSERT_EQ(digits, digits_expected);
    ASSERT_EQ(strings, strings_expected);
}

TEST(Parse, MixedTokens) {
    std::vector<uint64_t> digits;
    std::vector<std::string> strings;

    auto digit_ptr = [&digits](uint64_t digit) {
        digits.push_back(digit);
    };

    auto str_ptr = [&strings](const std::string& str) {
        strings.push_back(str);
    };

    std::string text = "abc 123 def456 789 xyz";
    parse(text, digit_ptr, str_ptr);

    std::vector<uint64_t> digits_expected = {123, 789};
    std::vector<std::string> strings_expected = {"abc", "def456", "xyz"};

    ASSERT_EQ(digits, digits_expected);
    ASSERT_EQ(strings, strings_expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}