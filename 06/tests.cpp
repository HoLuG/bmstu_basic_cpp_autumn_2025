#include <gtest/gtest.h>
#include "vector.hpp"
#include <vector>

TEST(VectorTest, DefaultConstructor) {
    Vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), size_t(0));
    EXPECT_GT(v.max_size(), size_t(0));
}

TEST(VectorTest, SizeConstructor) {
    Vector<double> v(7);
    EXPECT_EQ(v.size(), size_t(7));
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(v[0], 0);

    Vector<int> v1(4, 77);
    EXPECT_EQ(v1.size(), size_t(4));
    for (size_t i = 0; i < v1.size(); i++) {
        EXPECT_EQ(v1[i], 77);
    }
}

TEST(VectorTest, CopyConstructor) {
    Vector<double> v1(5, 22.5);
    Vector<double> v2(v1);

    EXPECT_EQ(v2.size(), size_t(5));
    EXPECT_EQ(v2[0], 22.5);
    
    v1[0] = 33.7;
    EXPECT_EQ(v2[0], 22.5);
}

TEST(VectorTest, MoveConstructor) {
    Vector<int> v1(6, 99);
    Vector<int> v2(std::move(v1));

    EXPECT_EQ(v2.size(), size_t(6));
    EXPECT_EQ(v2[0], 99);
    EXPECT_TRUE(v1.empty());
}

TEST(VectorTest, CopyAssignment) {
    Vector<int> v1(8, 15);
    Vector<int> v2;
    v2 = v1;

    EXPECT_EQ(v2.size(), size_t(8));
    EXPECT_EQ(v2[0], 15);
    
    v1[0] = 25;
    EXPECT_EQ(v2[0], 15);
}

TEST(VectorTest, MoveAssignment) {
    Vector<int> v1(4, -50);
    Vector<int> v2;
    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), size_t(4));
    EXPECT_EQ(v2[0], -50);
    EXPECT_TRUE(v1.empty());
}

TEST(VectorTest, ElementAccess) {
    Vector<int> v(4);
    std::vector<int> nums = {10, 20, 30, 40};
    for (size_t i = 0; i < 4; i++) {
        v[i] = nums[i];
    }

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 40);

    const Vector<int>& cv = v;
    EXPECT_EQ(cv[0], 10);
    EXPECT_EQ(cv.front(), 10);
}

TEST(VectorTest, Reserve) {
    Vector<int> v;
    v.reserve(15);
    EXPECT_EQ(v.capacity(), size_t(15));

    for (int i = 0; i < 7; i++) {
        v.push_back(i);
    }

    EXPECT_EQ(v.size(), size_t(7));
    EXPECT_EQ(v.capacity(), size_t(15));
}

TEST(VectorTest, ShrinkToFit) {
    Vector<int> v;
    v.reserve(20);
    for (int i = 0; i < 6; i++) {
        v.push_back(i);
    }

    EXPECT_GT(v.capacity(), v.size());
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());
}

TEST(VectorTest, PushBack) {
    Vector<int> v;
    std::vector<int> nums = {5, 10, 15, 20};
    for (int val : nums) {
        v.push_back(val);
    }

    EXPECT_EQ(v.size(), size_t(4));
    EXPECT_EQ(v[0], 5);
    EXPECT_EQ(v.back(), 20);
    EXPECT_EQ(v.data()[0], 5);
}

TEST(VectorTest, PopBack) {
    Vector<int> v;
    std::vector<int> nums = {12, 24, 36, 48, 60, 72};
    for (int val : nums) {
        v.push_back(val);
    }

    v.pop_back();
    EXPECT_EQ(v.size(), size_t(5));
    EXPECT_EQ(v.back(), 60);
}

TEST(VectorTest, Insert) {
    Vector<int> v;
    std::vector<int> nums = {7, 14, 28, 35};
    for (int val : nums) {
        v.push_back(val);
    }

    v.insert(2, 21);
    EXPECT_EQ(v.size(), size_t(5));
    EXPECT_EQ(v[2], 21);
    EXPECT_EQ(v[1], 14);
    EXPECT_EQ(v[3], 28);
}

TEST(VectorTest, Emplace) {
    Vector<int> v;
    std::vector<int> nums = {9, 18, 36, 45};
    for (int val : nums) {
        v.push_back(val);
    }

    v.emplace(2, 27);
    EXPECT_EQ(v.size(), size_t(5));
    EXPECT_EQ(v[2], 27);
}

TEST(VectorTest, Resize) {
    Vector<int> v;
    std::vector<int> nums = {11, 22, 33};
    for (int val : nums) {
        v.push_back(val);
    }

    v.resize(6);
    EXPECT_EQ(v.size(), size_t(6));
    EXPECT_EQ(v[0], 11);
    EXPECT_EQ(v[5], 0);

    v.resize(2);
    EXPECT_EQ(v.size(), size_t(2));
    EXPECT_EQ(v[1], 22);
}

TEST(VectorTest, Reverse) {
    Vector<int> v;
    std::vector<int> nums = {13, 26, 39, 52, 65, 78};
    for (int val : nums) {
        v.push_back(val);
    }

    v.reverse();
    EXPECT_EQ(v[0], 78);
    EXPECT_EQ(v.back(), 13);
}

TEST(VectorBoolTest, Constructor) {
    Vector<bool> v;
    EXPECT_TRUE(v.empty());

    Vector<bool> v2(15);
    EXPECT_EQ(v2.size(), size_t(15));
    EXPECT_FALSE(v2[0]);
}

TEST(VectorBoolTest, ConstructorWithValue) {
    Vector<bool> v(8, true);
    EXPECT_EQ(v.size(), size_t(8));
    EXPECT_TRUE(v[0]);
    EXPECT_NE(v.data(), nullptr);
}

TEST(VectorBoolTest, PushBackAndAccess) {
    Vector<bool> v;
    std::vector<bool> values = {false, true, false, false, true, true};
    for (bool val : values) {
        v.push_back(val);
    }

    EXPECT_EQ(v.size(), size_t(6));
    EXPECT_FALSE(v[0]);
    EXPECT_TRUE(v[1]);
    EXPECT_TRUE(v.back());
}

TEST(VectorBoolTest, SetValues) {
    Vector<bool> v(12);
    std::vector<size_t> true_positions = {1, 3, 6, 8, 11};
    for (size_t pos : true_positions) {
        v[pos] = true;
    }

    EXPECT_FALSE(v[0]);
    EXPECT_TRUE(v[1]);
    EXPECT_FALSE(v[2]);
    EXPECT_TRUE(v[3]);
}

TEST(VectorBoolTest, FrontBack) {
    Vector<bool> v;
    std::vector<bool> values = {false, true, false};
    for (bool val : values) {
        v.push_back(val);
    }

    EXPECT_FALSE(v.front());
    EXPECT_FALSE(v.back());

    v.front() = true;
    v.back() = true;
    EXPECT_TRUE(v.front());
    EXPECT_TRUE(v.back());
}

TEST(VectorBoolTest, PopBack) {
    Vector<bool> v;
    std::vector<bool> values = {false, true, false, true, false};
    for (bool val : values) {
        v.push_back(val);
    }

    v.pop_back();
    EXPECT_EQ(v.size(), size_t(4));
    EXPECT_TRUE(v.back());
}

TEST(VectorBoolTest, Resize) {
    Vector<bool> v;
    std::vector<bool> values = {false, true, false};
    for (bool val : values) {
        v.push_back(val);
    }

    v.resize(7);
    EXPECT_EQ(v.size(), size_t(7));
    EXPECT_FALSE(v[0]);
    EXPECT_FALSE(v[6]);

    v.resize(2);
    EXPECT_EQ(v.size(), size_t(2));
    EXPECT_TRUE(v[1]);
}

TEST(VectorBoolTest, Reserve) {
    Vector<bool> v;
    v.reserve(150);
    EXPECT_GE(v.capacity(), size_t(150));

    for (size_t i = 0; i < 75; i++) {
        v.push_back(i % 3 == 0);
    }

    EXPECT_EQ(v.size(), size_t(75));
    EXPECT_GE(v.capacity(), size_t(150));

    v.insert(1, true);
    EXPECT_EQ(v.size(), size_t(76));
    EXPECT_TRUE(v[1]);

    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());
}

TEST(VectorBoolTest, CopyConstructor) {
    Vector<bool> v1;
    std::vector<bool> values = {false, true, false, true, false, true};
    for (bool val : values) {
        v1.push_back(val);
    }

    Vector<bool> v2(v1);
    EXPECT_EQ(v1.size(), v2.size());
    EXPECT_FALSE(v1[0]);
    
    v2[0] = true;
    EXPECT_FALSE(v1[0]);
    EXPECT_TRUE(v2[0]);

    Vector<bool> v3;
    v3 = v1;
    EXPECT_EQ(v3.size(), v1.size());
    EXPECT_EQ(v3[1], v1[1]);
}

TEST(VectorBoolTest, MoveConstructor) {
    Vector<bool> v1;
    std::vector<bool> values = {false, true, false, true, false, true, false};
    for (bool val : values) {
        v1.push_back(val);
    }

    Vector<bool> v2(std::move(v1));
    EXPECT_EQ(v2.size(), size_t(7));
    EXPECT_FALSE(v2[0]);
    EXPECT_TRUE(v1.empty());

    Vector<bool> v3;
    v3 = std::move(v2);
    EXPECT_TRUE(v2.empty());
    EXPECT_EQ(v3.size(), size_t(7));
}

TEST(VectorBoolTest, Reverse) {
    Vector<bool> v;
    std::vector<bool> values = {false, true, false, true, false, true};
    for (bool val : values) {
        v.push_back(val);
    }

    v.reverse();
    EXPECT_TRUE(v[0]);
    EXPECT_FALSE(v.back());
}

struct Point {
    int x;
    int y;

    Point() : x(0), y(0) {}
    Point(int x_val, int y_val) : x(x_val), y(y_val) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

TEST(VectorPointTest, BasicOperations) {
    Vector<Point> v;
    std::vector<Point> points = {
        Point(10, 20),
        Point(30, 40),
        Point(50, 60),
        Point(70, 80)
    };

    for (const Point& p : points) {
        v.push_back(p);
    }

    EXPECT_EQ(v.size(), size_t(4));
    EXPECT_EQ(v.front(), Point(10, 20));
    EXPECT_EQ(v.back(), Point(70, 80));

    v.emplace_back(90, 100);
    EXPECT_EQ(v.size(), size_t(5));
    EXPECT_EQ(v.back(), Point(90, 100));

    Vector<Point> v_copy(v);
    EXPECT_EQ(v_copy.size(), size_t(5));
    EXPECT_EQ(v_copy[0], Point(10, 20));

    v.reverse();
    EXPECT_EQ(v[0], Point(90, 100));
    EXPECT_EQ(v.back(), Point(10, 20));
}



TEST(VectorTest, ResizeWithValue) {
    Vector<int> v;
    v.push_back(100);
    v.push_back(200);
    
    v.resize(5, 300);
    EXPECT_EQ(v.size(), size_t(5));
    EXPECT_EQ(v[0], 100);
    EXPECT_EQ(v[2], 300);
    EXPECT_EQ(v[4], 300);
}



TEST(VectorTest, EmptyVectorExceptions) {
    Vector<int> v;
    EXPECT_THROW(v.front(), std::out_of_range);
    EXPECT_THROW(v.back(), std::out_of_range);
}

TEST(VectorBoolTest, EmptyVectorExceptions) {
    Vector<bool> v;
    EXPECT_THROW(v.front(), std::out_of_range);
    EXPECT_THROW(v.back(), std::out_of_range);
}

TEST(VectorTest, OutOfRangeExceptions) {
    Vector<int> v;
    v.push_back(1);
    EXPECT_THROW(v.insert(2, 10), std::out_of_range);
    EXPECT_THROW(v.emplace(2, 10), std::out_of_range);
}

TEST(VectorTest, CapacityGrowth) {
    Vector<int> v;
    auto cap0 = v.capacity();
    for (int i = 0; i < 100; i++) {
        v.push_back(i);
    }
    EXPECT_GE(v.capacity(), v.size());
    EXPECT_NE(v.capacity(), cap0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}