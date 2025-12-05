#include "allocator.hpp"
#include <gtest/gtest.h>

TEST(Allocator, BasicAllocation) {
    size_t max_size = 32;
    Allocator* a = init_allocator(max_size);

    ASSERT_NE(a, nullptr);
    ASSERT_EQ(a->max_size, max_size);
    ASSERT_EQ(a->offset, 0u);
    ASSERT_NE(a->buf, nullptr);

    clear(a);
}

TEST(Allocator, WriteToAllocatedMemory) {
    Allocator* a = init_allocator(25);
    char* ptr = alloc(a, 12);
    ASSERT_NE(ptr, nullptr);

    for (int i = 0; i < 12; ++i) {
    ptr[i] = static_cast<char>('a' + i);
    }

    ASSERT_EQ(ptr[0], 'a');
    ASSERT_EQ(ptr[11], 'l');

    clear(a);
}

TEST(Allocator, AllocReturnsPointer) {
    Allocator* a = init_allocator(19);
    char* ptr = alloc(a, 5);

    ASSERT_NE(ptr, nullptr);
    ASSERT_EQ(a->offset, 5u);

    clear(a);
}

TEST(Allocator, AllocZeroSize) {
    Allocator* a = init_allocator(13);
    char* ptr = alloc(a, 0);

    ASSERT_NE(ptr, nullptr);
    ASSERT_EQ(a->offset, 0u);

    clear(a);
}

TEST(Allocator, AllocReturnsNullptr) {
    Allocator* a = init_allocator(40);
    char* ptr = alloc(a, 41);

    ASSERT_EQ(ptr, nullptr);
    ASSERT_EQ(a->offset, 0u);

    clear(a);
}

TEST(Allocator, AllocExactSizeWithSeveralCalls) {
    size_t max_size = 16;
    Allocator* a = init_allocator(max_size);

    char* p1 = alloc(a, 4);
    char* p2 = alloc(a, 12);

    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);
    ASSERT_EQ(a->offset, max_size);

    char* p3 = alloc(a, 1);
    ASSERT_EQ(p3, nullptr);
    ASSERT_EQ(a->offset, max_size);

    clear(a);
}

TEST(Allocator, RepeatAlloc) {
    size_t max_size = 21;
    Allocator* a = init_allocator(max_size);

    char* p1 = alloc(a, 9);
    char* p2 = alloc(a, 6);
    char* p3 = alloc(a, 6);

    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);
    ASSERT_NE(p3, nullptr);
    ASSERT_EQ(a->offset, max_size);

    clear(a);
}

TEST(Allocator, ResetAllowsReuse) {
    Allocator* a = init_allocator(15);

    char* p1 = alloc(a, 11);
    ASSERT_NE(p1, nullptr);
    ASSERT_EQ(a->offset, 11u);

    reset(a);
    ASSERT_EQ(a->offset, 0u);

    char* p2 = alloc(a, 15);
    ASSERT_NE(p2, nullptr);
    ASSERT_EQ(a->offset, 15u);

    clear(a);
}

TEST(Allocator, AllocAfterReset) {
    size_t max_size = 18;
    Allocator* a = init_allocator(max_size);

    char* first = alloc(a, 10);
    ASSERT_NE(first, nullptr);
    ASSERT_EQ(a->offset, 10u);

    reset(a);

    char* second = alloc(a, 17);
    ASSERT_NE(second, nullptr);
    ASSERT_EQ(a->offset, 17u);

    clear(a);
}

TEST(Allocator, RepeatInitAllocator) {
    Allocator* a = init_allocator(20);
    char* p1 = alloc(a, 10);
    ASSERT_NE(p1, nullptr);

    Allocator* same = init_allocator(a, 8);
    ASSERT_EQ(same, a);
    ASSERT_EQ(a->max_size, 8u);
    ASSERT_EQ(a->offset, 0u);

    char* p2 = alloc(a, 8);
    ASSERT_NE(p2, nullptr);
    ASSERT_EQ(a->offset, 8u);

    size_t new_max_size = 12;
    Allocator* same2 = init_allocator(a, new_max_size);
    ASSERT_EQ(same2, a);
    ASSERT_EQ(a->max_size, new_max_size);
    ASSERT_EQ(a->offset, 0u);

    clear(a);
}

TEST(Allocator, InitAllocatorWithNullPointer) {
    Allocator* a = nullptr;

    a = init_allocator(a, 14);
    ASSERT_NE(a, nullptr);
    ASSERT_EQ(a->max_size, 14u);
    ASSERT_EQ(a->offset, 0u);

    clear(a);
}

TEST(Allocator, MultipleResets) {
    Allocator* a = init_allocator(30);

    ASSERT_NE(alloc(a, 10), nullptr);
    reset(a);
    ASSERT_EQ(a->offset, 0u);

    ASSERT_NE(alloc(a, 5), nullptr);
    reset(a);
    ASSERT_EQ(a->offset, 0u);

    ASSERT_NE(alloc(a, 30), nullptr);
    ASSERT_EQ(a->offset, 30u);

    clear(a);
}

TEST(Allocator, AllocOnNull) {
    char* ptr = alloc(nullptr, 10);
    ASSERT_EQ(ptr, nullptr);
}

TEST(Allocator, ResetOnNull) {
    reset(nullptr);
    SUCCEED();
}

TEST(Allocator, ClearOnNull) {
    clear(nullptr);
    SUCCEED();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
