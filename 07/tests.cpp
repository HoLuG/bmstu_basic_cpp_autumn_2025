#include <gtest/gtest.h>
#include "format.hpp"
#include <string>

TEST(FormatTest, BasicExample) {
    auto text = format("{1}+{1} = {0}", 2, "one");
    EXPECT_EQ(text, "one+one = 2");
}

TEST(FormatTest, DifferentTypes) {
    auto text = format("{0} {1} {2} {3}", 42, 3.14, "hello", 'c');
    EXPECT_EQ(text, "42 3.14 hello c");
}

TEST(FormatTest, MultipleArgumentReuse) {
    auto text = format("{0} {1} {0} {1} {0}", "foo", "bar");
    EXPECT_EQ(text, "foo bar foo bar foo");
}

TEST(FormatTest, NoPlaceholders) {
    auto text = format("Just plain text", 1, 2, 3);
    EXPECT_EQ(text, "Just plain text");
}

TEST(FormatTest, EmptyStringNoArgs) {
    auto text = format("");
    EXPECT_EQ(text, "");
}

TEST(FormatTest, UnorderedIndices) {
    auto text = format("{2}{0}{1}", "first", "second", "third");
    EXPECT_EQ(text, "thirdfirstsecond");
}

TEST(FormatExceptionTest, UnclosedBrace) {
    EXPECT_THROW({
        format("{0", 1);
    }, InvalidBracesException);
}

TEST(FormatExceptionTest, UnmatchedClosingBrace) {
    EXPECT_THROW({
        format("test}", 1);
    }, InvalidBracesException);
}

TEST(FormatExceptionTest, EmptyBraces) {
    EXPECT_THROW({
        format("{}", 1);
    }, InvalidBracesException);
}

TEST(FormatExceptionTest, NestedBraces) {
    EXPECT_THROW({
        format("{{0}}", 1);
    }, InvalidBracesException);
}

TEST(FormatExceptionTest, NonDigitInBraces) {
    EXPECT_THROW({
        format("{a}", 1);
    }, InvalidBracesException);
}

TEST(FormatExceptionTest, IndexOutOfRange) {
    EXPECT_THROW({
        format("{1}", 1);
    }, ArgumentIndexException);
}

TEST(FormatExceptionTest, NoArgumentsButPlaceholder) {
    EXPECT_THROW({
        format("{0}");
    }, ArgumentIndexException);
}

TEST(FormatExceptionTest, ClosingBraceInMiddle) {
    EXPECT_THROW({
        format("test } more", 1);
    }, InvalidBracesException);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
