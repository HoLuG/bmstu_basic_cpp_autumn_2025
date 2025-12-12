#include "phonetic.hpp"
#include <gtest/gtest.h>

TEST(PhoneticAlgorithm, ConvertTextToSound_Ashcraft) {
    EXPECT_EQ(convertTextToSound("Ashcraft"), std::string("A261"));
}

TEST(PhoneticAlgorithm, ConvertTextToSound_Ashcroft) {
    EXPECT_EQ(convertTextToSound("Ashcroft"), std::string("A261"));
}

TEST(PhoneticAlgorithm, IsEqual_AshcraftAshcroft) {
    EXPECT_TRUE(isEqual("Ashcraft", "Ashcroft"));
}

TEST(PhoneticAlgorithm, IsEqual_GaussGhosh) {
    EXPECT_TRUE(isEqual("Gauss", "Ghosh"));
}

TEST(PhoneticAlgorithm, IsEqual_KnuthKant) {
    EXPECT_TRUE(isEqual("Knuth", "Kant"));
}

TEST(PhoneticAlgorithm, IsEqual_HelloWorld) {
    EXPECT_FALSE(isEqual("Hello", "World"));
}

TEST(PhoneticAlgorithm, ConvertTextToSound_SingleLetter) {
    EXPECT_EQ(convertTextToSound("A"), std::string("A000"));
}

TEST(PhoneticAlgorithm, ConvertTextToSound_TwoLetters) {
    EXPECT_EQ(convertTextToSound("Ab"), std::string("A100"));
}

TEST(PhoneticAlgorithm, ConvertTextToSound_WithH) {
    EXPECT_EQ(convertTextToSound("Ash"), std::string("A200"));
}

TEST(PhoneticAlgorithm, ConvertTextToSound_WithW) {
    EXPECT_EQ(convertTextToSound("Aw"), std::string("A000"));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
