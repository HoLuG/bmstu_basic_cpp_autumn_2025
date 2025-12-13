#include <gtest/gtest.h>
#include "matrix.hpp"
#include <cstdint>

TEST(MatrixTest, BasicConstructor) {
    const size_t rows = 5;
    const size_t cols = 3;
    Matrix m(rows, cols);
    ASSERT_EQ(m.getRows(), rows);
    ASSERT_EQ(m.getColumns(), cols);
}

TEST(MatrixTest, ElementAssignmentAndAccess) {
    Matrix m(3, 4);
    m[1][2] = 5;
    ASSERT_EQ(m[1][2], 5);

    m[0][0] = 10;
    m[2][3] = -7;
    ASSERT_EQ(m[0][0], 10);
    ASSERT_EQ(m[2][3], -7);
}

TEST(MatrixTest, CopyConstructor) {
    Matrix original(4, 5);
    original[0][0] = 1;
    original[1][1] = 2;
    original[2][2] = 3;
    original[3][3] = 4;

    Matrix copy(original);
    ASSERT_EQ(copy.getRows(), size_t(4));
    ASSERT_EQ(copy.getColumns(), size_t(5));
    ASSERT_EQ(copy[0][0], 1);
    ASSERT_EQ(copy[1][1], 2);
    ASSERT_EQ(copy[2][2], 3);
    ASSERT_EQ(copy[3][3], 4);

    original[0][0] = 99;
    ASSERT_EQ(copy[0][0], 1);
}

TEST(MatrixTest, AssignmentOperator) {
    Matrix m1(2, 3);
    m1[0][0] = 7;
    m1[0][1] = 8;
    m1[0][2] = 9;
    m1[1][0] = 10;
    m1[1][1] = 11;
    m1[1][2] = 12;

    Matrix m2(1, 1);
    m2 = m1;

    ASSERT_EQ(m2.getRows(), size_t(2));
    ASSERT_EQ(m2.getColumns(), size_t(3));
    ASSERT_EQ(m2[0][0], 7);
    ASSERT_EQ(m2[1][2], 12);

    m1[0][0] = 100;
    ASSERT_EQ(m2[0][0], 7);
}

TEST(MatrixTest, ScalarMultiplication) {
    Matrix m(3, 3);
    m[0][0] = 2;
    m[0][1] = 4;
    m[0][2] = 6;
    m[1][0] = 8;
    m[1][1] = 10;
    m[1][2] = 12;
    m[2][0] = 14;
    m[2][1] = 16;
    m[2][2] = 18;

    m *= 3;

    ASSERT_EQ(m[0][0], 6);
    ASSERT_EQ(m[0][1], 12);
    ASSERT_EQ(m[0][2], 18);
    ASSERT_EQ(m[1][0], 24);
    ASSERT_EQ(m[1][1], 30);
    ASSERT_EQ(m[1][2], 36);
    ASSERT_EQ(m[2][0], 42);
    ASSERT_EQ(m[2][1], 48);
    ASSERT_EQ(m[2][2], 54);
}

TEST(MatrixTest, ScalarMultiplicationNegative) {
    Matrix m(2, 2);
    m[0][0] = 5;
    m[0][1] = -3;
    m[1][0] = 7;
    m[1][1] = 9;

    m *= -2;

    ASSERT_EQ(m[0][0], -10);
    ASSERT_EQ(m[0][1], 6);
    ASSERT_EQ(m[1][0], -14);
    ASSERT_EQ(m[1][1], -18);
}

TEST(MatrixTest, MatrixAddition) {
    Matrix m1(2, 3);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[0][2] = 3;
    m1[1][0] = 4;
    m1[1][1] = 5;
    m1[1][2] = 6;

    Matrix m2(2, 3);
    m2[0][0] = 10;
    m2[0][1] = 20;
    m2[0][2] = 30;
    m2[1][0] = 40;
    m2[1][1] = 50;
    m2[1][2] = 60;

    Matrix m3 = m1 + m2;

    ASSERT_EQ(m3[0][0], 11);
    ASSERT_EQ(m3[0][1], 22);
    ASSERT_EQ(m3[0][2], 33);
    ASSERT_EQ(m3[1][0], 44);
    ASSERT_EQ(m3[1][1], 55);
    ASSERT_EQ(m3[1][2], 66);
}

TEST(MatrixTest, MatrixAdditionWithNegative) {
    Matrix m1(3, 2);
    m1[0][0] = 15;
    m1[0][1] = -5;
    m1[1][0] = 20;
    m1[1][1] = -10;
    m1[2][0] = 25;
    m1[2][1] = -15;

    Matrix m2(3, 2);
    m2[0][0] = 5;
    m2[0][1] = 10;
    m2[1][0] = -5;
    m2[1][1] = 15;
    m2[2][0] = 0;
    m2[2][1] = -5;

    Matrix m3 = m1 + m2;

    ASSERT_EQ(m3[0][0], 20);
    ASSERT_EQ(m3[0][1], 5);
    ASSERT_EQ(m3[1][0], 15);
    ASSERT_EQ(m3[1][1], 5);
    ASSERT_EQ(m3[2][0], 25);
    ASSERT_EQ(m3[2][1], -20);
}

TEST(MatrixTest, AdditionDifferentDimensions) {
    Matrix m1(2, 2);
    Matrix m2(3, 2);
    Matrix m3(2, 3);

    ASSERT_THROW(m1 + m2, std::invalid_argument);
    ASSERT_THROW(m1 + m3, std::invalid_argument);
}

TEST(MatrixTest, EqualityOperator) {
    Matrix m1(3, 2);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 3;
    m1[1][1] = 4;
    m1[2][0] = 5;
    m1[2][1] = 6;

    Matrix m2(3, 2);
    m2[0][0] = 1;
    m2[0][1] = 2;
    m2[1][0] = 3;
    m2[1][1] = 4;
    m2[2][0] = 5;
    m2[2][1] = 6;

    Matrix m3(3, 2);
    m3[0][0] = 1;
    m3[0][1] = 2;
    m3[1][0] = 3;
    m3[1][1] = 99;
    m3[2][0] = 5;
    m3[2][1] = 6;

    ASSERT_TRUE(m1 == m2);
    ASSERT_FALSE(m1 == m3);
}

TEST(MatrixTest, InequalityOperator) {
    Matrix m1(2, 2);
    m1[0][0] = 10;
    m1[0][1] = 20;
    m1[1][0] = 30;
    m1[1][1] = 40;

    Matrix m2(2, 2);
    m2[0][0] = 10;
    m2[0][1] = 20;
    m2[1][0] = 30;
    m2[1][1] = 40;

    Matrix m3(2, 2);
    m3[0][0] = 10;
    m3[0][1] = 20;
    m3[1][0] = 30;
    m3[1][1] = 41;

    Matrix m4(3, 2);

    ASSERT_FALSE(m1 != m2);
    ASSERT_TRUE(m1 != m3);
    ASSERT_TRUE(m1 != m4);
}

TEST(MatrixTest, OutOfRangeRow) {
    Matrix m(4, 5);
    ASSERT_THROW(m[4][0] = 1, std::out_of_range);
    ASSERT_THROW(m[5][0] = 1, std::out_of_range);
    ASSERT_NO_THROW(m[3][4] = 1);
}

TEST(MatrixTest, OutOfRangeColumn) {
    Matrix m(4, 5);
    ASSERT_THROW(m[0][5] = 1, std::out_of_range);
    ASSERT_THROW(m[1][10] = 1, std::out_of_range);
    ASSERT_NO_THROW(m[3][4] = 1);
}

TEST(MatrixTest, ConstAccess) {
    Matrix m(3, 3);
    m[0][0] = 11;
    m[0][1] = 22;
    m[0][2] = 33;
    m[1][0] = 44;
    m[1][1] = 55;
    m[1][2] = 66;
    m[2][0] = 77;
    m[2][1] = 88;
    m[2][2] = 99;

    const Matrix& const_m = m;

    ASSERT_EQ(const_m[0][0], 11);
    ASSERT_EQ(const_m[1][1], 55);
    ASSERT_EQ(const_m[2][2], 99);

    double x = const_m[1][2];
    ASSERT_EQ(x, 66);
}

TEST(MatrixTest, LargeMatrix) {
    Matrix m(10, 15);
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 15; j++) {
            m[i][j] = static_cast<int32_t>(i * 15 + j);
        }
    }

    ASSERT_EQ(m[0][0], 0);
    ASSERT_EQ(m[5][7], 82);
    ASSERT_EQ(m[9][14], 149);
}

TEST(MatrixTest, ZeroMatrix) {
    Matrix m(3, 4);
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 4; j++) {
            ASSERT_EQ(m[i][j], 0);
        }
    }
}

TEST(MatrixTest, SelfAssignment) {
    Matrix m(2, 2);
    m[0][0] = 5;
    m[0][1] = 6;
    m[1][0] = 7;
    m[1][1] = 8;

    m = m;

    ASSERT_EQ(m[0][0], 5);
    ASSERT_EQ(m[1][1], 8);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}