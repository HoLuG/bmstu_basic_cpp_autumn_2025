#include <gtest/gtest.h>
#include "BigInt.hpp"
#include <sstream>

TEST(BigIntTest, DefaultConstructor) {
    BigInt num;
    ASSERT_EQ(num.ToString(), "0");
}

TEST(BigIntTest, Int32Constructor) {
    ASSERT_EQ(BigInt(0).ToString(), "0");
    ASSERT_EQ(BigInt(789).ToString(), "789");
    ASSERT_EQ(BigInt(-321).ToString(), "-321");
    ASSERT_EQ(BigInt(2147483647).ToString(), "2147483647");
    ASSERT_EQ(BigInt(-2147483648).ToString(), "-2147483648");
}

TEST(BigIntTest, StringConstructor) {
    ASSERT_EQ(BigInt("").ToString(), "0");
    ASSERT_EQ(BigInt("987").ToString(), "987");
    ASSERT_EQ(BigInt("-654").ToString(), "-654");
    ASSERT_EQ(BigInt("987654321098765432109876543210").ToString(), "987654321098765432109876543210");
    ASSERT_EQ(BigInt("-987654321098765432109876543210").ToString(), "-987654321098765432109876543210");
    ASSERT_EQ(BigInt("00000123").ToString(), "123");
}

TEST(BigIntTest, CopyConstructor) {
    BigInt num1("987654321098765432109876543210");
    BigInt num2(num1);
    ASSERT_EQ(num2.ToString(), "987654321098765432109876543210");

    num1 = BigInt(42);
    ASSERT_EQ(num2.ToString(), "987654321098765432109876543210");
}

TEST(BigIntTest, MoveConstructor) {
    BigInt num1("555555555555555555555555555555");
    BigInt num2(std::move(num1));
    ASSERT_EQ(num2.ToString(), "555555555555555555555555555555");
}

TEST(BigIntTest, CopyAssignment) {
    BigInt num1("111222333444555666777888999000");
    BigInt num2;
    num2 = num1;
    ASSERT_EQ(num2.ToString(), "111222333444555666777888999000");

    num1 = BigInt(999);
    ASSERT_EQ(num2.ToString(), "111222333444555666777888999000");
}

TEST(BigIntTest, MoveAssignment) {
    BigInt num1("777888999000111222333444555666");
    BigInt num2;
    num2 = std::move(num1);
    ASSERT_EQ(num2.ToString(), "777888999000111222333444555666");
}

TEST(BigIntTest, Int32Assignment) {
    BigInt num;
    num = 777;
    ASSERT_EQ(num.ToString(), "777");
    num = -888;
    ASSERT_EQ(num.ToString(), "-888");
}

TEST(BigIntTest, UnaryMinus) {
    ASSERT_EQ((-BigInt("789")).ToString(), "-789");
    ASSERT_EQ((-BigInt("-321")).ToString(), "321");
    ASSERT_EQ((-BigInt("0")).ToString(), "0");
}

TEST(BigIntTest, AdditionBigInt) {
    ASSERT_EQ((BigInt("789") + BigInt("321")).ToString(), "1110");
    ASSERT_EQ((BigInt("-789") + BigInt("321")).ToString(), "-468");
    ASSERT_EQ((BigInt("789") + BigInt("-321")).ToString(), "468");
    ASSERT_EQ((BigInt("-789") + BigInt("-321")).ToString(), "-1110");
    ASSERT_EQ((BigInt("1999") + BigInt("1")).ToString(), "2000");
}

TEST(BigIntTest, AdditionInt32) {
    ASSERT_EQ((BigInt("500") + 300).ToString(), "800");
    ASSERT_EQ((BigInt("-500") + 300).ToString(), "-200");
    ASSERT_EQ((BigInt("500") + (-300)).ToString(), "200");
}

TEST(BigIntTest, PlusEquals) {
    BigInt num("1000");
    num += BigInt("2000");
    ASSERT_EQ(num.ToString(), "3000");
    num = BigInt("777");
    num += 223;
    ASSERT_EQ(num.ToString(), "1000");
}

TEST(BigIntTest, SubBigInt) {
    ASSERT_EQ((BigInt("1000") - BigInt("333")).ToString(), "667");
    ASSERT_EQ((BigInt("333") - BigInt("1000")).ToString(), "-667");
    ASSERT_EQ((BigInt("-333") - BigInt("1000")).ToString(), "-1333");
    ASSERT_EQ((BigInt("1000") - BigInt("-333")).ToString(), "1333");
    ASSERT_EQ((BigInt("987654321098765432109876543210") - BigInt("987654321098765432109876543210")).ToString(), "0");
}

TEST(BigIntTest, SubInt32) {
    ASSERT_EQ((BigInt("888") - 222).ToString(), "666");
    ASSERT_EQ((BigInt("222") - 888).ToString(), "-666");
    ASSERT_EQ((BigInt("888") - (-222)).ToString(), "1110");
}

TEST(BigIntTest, MinusEquals) {
    BigInt num("2000");
    num -= BigInt("500");
    ASSERT_EQ(num.ToString(), "1500");
    num = BigInt("2000");
    num -= 500;
    ASSERT_EQ(num.ToString(), "1500");
}

TEST(BigIntTest, MultiplicationBigInt) {
    ASSERT_EQ((BigInt("777") * BigInt("111")).ToString(), "86247");
    ASSERT_EQ((BigInt("-777") * BigInt("111")).ToString(), "-86247");
    ASSERT_EQ((BigInt("-777") * BigInt("-111")).ToString(), "86247");
    ASSERT_EQ((BigInt("0") * BigInt("999")).ToString(), "0");
    ASSERT_EQ((BigInt("555555555") * BigInt("222222222")).ToString(), "123456789876543210");
}

TEST(BigIntTest, MultiplicationInt32) {
    ASSERT_EQ((BigInt("333") * 222).ToString(), "73926");
    ASSERT_EQ((BigInt("-333") * 222).ToString(), "-73926");
    ASSERT_EQ((BigInt("-333") * (-222)).ToString(), "73926");
    ASSERT_EQ((BigInt("0") * 777).ToString(), "0");
}

TEST(BigIntTest, TimesEquals) {
    BigInt num("111");
    num *= BigInt("222");
    ASSERT_EQ(num.ToString(), "24642");
    num = BigInt("111");
    num *= 222;
    ASSERT_EQ(num.ToString(), "24642");
}

TEST(BigIntTest, Equality) {
    ASSERT_TRUE(BigInt("999") == BigInt("999"));
    ASSERT_TRUE(BigInt("999") == BigInt("000999"));
    ASSERT_TRUE(BigInt("-999") == BigInt("-999"));
    ASSERT_TRUE(BigInt("-0") == BigInt("0"));
    ASSERT_FALSE(BigInt("999") == BigInt("888"));
    ASSERT_FALSE(BigInt("999") == BigInt("-999"));
}

TEST(BigIntTest, Inequality) {
    ASSERT_TRUE(BigInt("777") != BigInt("888"));
    ASSERT_TRUE(BigInt("777") != BigInt("-777"));
    ASSERT_FALSE(BigInt("777") != BigInt("777"));
    ASSERT_FALSE(BigInt("0") != BigInt("0"));
}

TEST(BigIntTest, LessThan) {
    ASSERT_TRUE(BigInt("555") < BigInt("666"));
    ASSERT_TRUE(BigInt("-555") < BigInt("555"));
    ASSERT_TRUE(BigInt("-666") < BigInt("-555"));
    ASSERT_FALSE(BigInt("666") < BigInt("555"));
    ASSERT_FALSE(BigInt("555") < BigInt("555"));
}

TEST(BigIntTest, LessThanOrEqual) {
    ASSERT_TRUE(BigInt("444") <= BigInt("555"));
    ASSERT_TRUE(BigInt("-444") <= BigInt("444"));
    ASSERT_TRUE(BigInt("444") <= BigInt("444"));
    ASSERT_FALSE(BigInt("555") <= BigInt("444"));
    ASSERT_FALSE(BigInt("444") <= BigInt("-444"));
}

TEST(BigIntTest, GreaterThan) {
    ASSERT_TRUE(BigInt("888") > BigInt("777"));
    ASSERT_TRUE(BigInt("777") > BigInt("-777"));
    ASSERT_TRUE(BigInt("-777") > BigInt("-888"));
    ASSERT_FALSE(BigInt("777") > BigInt("888"));
    ASSERT_FALSE(BigInt("777") > BigInt("777"));
}

TEST(BigIntTest, GreaterThanOrEqual) {
    ASSERT_TRUE(BigInt("333") >= BigInt("222"));
    ASSERT_TRUE(BigInt("222") >= BigInt("-222"));
    ASSERT_TRUE(BigInt("222") >= BigInt("222"));
    ASSERT_FALSE(BigInt("222") >= BigInt("333"));
    ASSERT_FALSE(BigInt("-222") >= BigInt("222"));
}

TEST(BigIntTest, OutputOperator) {
    std::ostringstream oss;
    oss << BigInt("999");
    ASSERT_EQ(oss.str(), "999");

    oss.str("");
    oss << BigInt("-888");
    ASSERT_EQ(oss.str(), "-888");

    oss.str("");
    oss << BigInt("987654321098765432109876543210");
    ASSERT_EQ(oss.str(), "987654321098765432109876543210");
}

TEST(BigIntTest, ComplexOperations) {
    BigInt a = 42;
    BigInt b("987654321098765432109876543210");
    BigInt c = a * b + 100;
    ASSERT_EQ(c.ToString(), "41481481486148148148614814814920");

    BigInt d;
    d = std::move(c);
    ASSERT_EQ(d.ToString(), "41481481486148148148614814814920");

    a = d + b;
    ASSERT_EQ(a.ToString(), "42469135807246913580724691358130");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}