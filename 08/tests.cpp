#include <gtest/gtest.h>
#include "btree.hpp"
#include <string>
#include <vector>
#include <map>
#include <random>
#include <algorithm>

TEST(BSTTest, EmptyTree) {
    bst<int, std::string> tree;
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0U);
    EXPECT_EQ(tree.begin(), tree.end());
}

TEST(BSTTest, InsertAndAccess) {
    bst<int, std::string> tree;
    
    auto [it1, ok1] = tree.insert({5, "five"});
    EXPECT_TRUE(ok1);
    EXPECT_EQ(it1->first, 5);
    EXPECT_EQ(it1->second, "five");
    
    auto [it2, ok2] = tree.insert({3, "three"});
    EXPECT_TRUE(ok2);
    
    auto [it3, ok3] = tree.insert({5, "duplicate"});
    EXPECT_FALSE(ok3);
    EXPECT_EQ(it3->second, "five");
    
    EXPECT_EQ(tree.size(), 2U);
    EXPECT_EQ(tree[5], "five");
    EXPECT_EQ(tree[3], "three");
    
    tree[10] = "ten";
    EXPECT_EQ(tree[10], "ten");
    EXPECT_EQ(tree.size(), 3U);
}

TEST(BSTTest, AtThrows) {
    bst<int, std::string> tree;
    tree.insert({1, "one"});
    tree.insert({2, "two"});
    
    EXPECT_EQ(tree.at(1), "one");
    EXPECT_EQ(tree.at(2), "two");
    EXPECT_THROW(tree.at(99), std::out_of_range);
    
    tree.at(1) = "modified";
    EXPECT_EQ(tree.at(1), "modified");
}

TEST(BSTTest, FindAndContains) {
    bst<int, std::string> tree;
    tree.insert({1, "one"});
    tree.insert({2, "two"});
    tree.insert({3, "three"});
    
    EXPECT_TRUE(tree.contains(1));
    EXPECT_TRUE(tree.contains(2));
    EXPECT_FALSE(tree.contains(99));
    
    auto it = tree.find(2);
    EXPECT_NE(it, tree.end());
    EXPECT_EQ(it->first, 2);
    EXPECT_EQ(it->second, "two");
    
    EXPECT_EQ(tree.find(99), tree.end());
}

TEST(BSTTest, Erase) {
    bst<int, std::string> tree;
    tree.insert({5, "root"});
    tree.insert({3, "left"});
    tree.insert({7, "right"});
    tree.insert({2, "two"});
    tree.insert({4, "four"});
    
    tree.erase(5);
    EXPECT_EQ(tree.size(), 4U);
    EXPECT_FALSE(tree.contains(5));
    EXPECT_TRUE(tree.contains(3));
    EXPECT_TRUE(tree.contains(7));
    
    tree.erase(3);
    EXPECT_EQ(tree.size(), 3U);
    EXPECT_FALSE(tree.contains(3));
    EXPECT_TRUE(tree.contains(2));
    EXPECT_TRUE(tree.contains(4));
}

TEST(BSTTest, EraseByIterator) {
    bst<int, std::string> tree;
    tree.insert({1, "one"});
    tree.insert({2, "two"});
    tree.insert({3, "three"});
    
    auto it = tree.find(2);
    ASSERT_NE(it, tree.end());
    auto next = tree.erase(it);
    
    ASSERT_EQ(tree.size(), 2U);
    EXPECT_FALSE(tree.contains(2));
    ASSERT_NE(next, tree.end());
    EXPECT_EQ(next->first, 3);
    
    it = tree.find(3);
    ASSERT_NE(it, tree.end());
    next = tree.erase(it);
    EXPECT_EQ(next, tree.end());
}

TEST(BSTTest, EraseIteratorWithTwoChildren) {
    bst<int, std::string> tree;
    tree.insert({4, "four"});
    tree.insert({2, "two"});
    tree.insert({6, "six"});
    tree.insert({1, "one"});
    tree.insert({3, "three"});
    tree.insert({5, "five"});
    tree.insert({7, "seven"});
    
    auto it = tree.find(2);
    ASSERT_NE(it, tree.end());
    
    auto next = tree.erase(it);
    ASSERT_NE(next, tree.end());
    EXPECT_EQ(next->first, 3);
    
    std::vector<int> keys;
    for (auto iter = tree.begin(); iter != tree.end(); iter++) {
        keys.push_back(iter->first);
    }
    EXPECT_EQ(keys, std::vector<int>({1, 3, 4, 5, 6, 7}));
    
    for (auto iter = tree.begin(); iter != tree.end(); iter++) {
        auto check = iter;
        check++;
        if (check != tree.end()) {
            check--;
            EXPECT_EQ(check->first, iter->first);
        }
    }
}

TEST(BSTTest, IteratorTraversal) {
    bst<int, std::string> tree;
    tree.insert({5, "five"});
    tree.insert({3, "three"});
    tree.insert({7, "seven"});
    tree.insert({2, "two"});
    tree.insert({4, "four"});
    
    std::vector<int> forward;
    for (auto it = tree.begin(); it != tree.end(); it++) {
        forward.push_back(it->first);
    }
    EXPECT_EQ(forward, std::vector<int>({2, 3, 4, 5, 7}));
    
    std::vector<int> reverse;
    for (auto it = tree.rbegin(); it != tree.rend(); it++) {
        reverse.push_back(it->first);
    }
    EXPECT_EQ(reverse, std::vector<int>({7, 5, 4, 3, 2}));
}

TEST(BSTTest, RangeBasedFor) {
    bst<int, std::string> tree;
    tree[3] = "three";
    tree[1] = "one";
    tree[2] = "two";
    
    std::vector<int> keys;
    for (const auto& pair : tree) {
        keys.push_back(pair.first);
    }
    EXPECT_EQ(keys, std::vector<int>({1, 2, 3}));
}

TEST(BSTTest, IteratorOperators) {
    bst<int, std::string> tree;
    tree[1] = "one";
    tree[2] = "two";
    tree[3] = "three";
    
    auto it = tree.begin();
    auto old = it++;
    EXPECT_EQ(old->first, 1);
    EXPECT_EQ(it->first, 2);
    
    it = tree.end();
    it--;
    EXPECT_EQ(it->first, 3);
    it--;
    EXPECT_EQ(it->first, 2);
    EXPECT_EQ(it, tree.find(2));
}

TEST(BSTTest, EraseAndIterate) {
    bst<int, std::string> tree;
    for (int i = 1; i <= 10; i++) {
        tree[i] = "val" + std::to_string(i);
    }
    
    tree.erase(3);
    tree.erase(7);
    tree.erase(1);
    
    std::vector<int> remaining;
    for (const auto& p : tree) {
        remaining.push_back(p.first);
    }
    
    EXPECT_EQ(tree.size(), 7U);
    EXPECT_EQ(remaining, std::vector<int>({2, 4, 5, 6, 8, 9, 10}));
}

TEST(BSTTest, EraseWithNonDirectSuccessor) {
    bst<int, std::string> tree;
    tree.insert({10, "ten"});
    tree.insert({5, "five"});
    tree.insert({15, "fifteen"});
    tree.insert({12, "twelve"});
    tree.insert({11, "eleven"});
    tree.insert({18, "eighteen"});
    
    tree.erase(10);
    
    ASSERT_EQ(tree.size(), 5U);
    EXPECT_FALSE(tree.contains(10));
    
    std::vector<int> keys;
    for (const auto& p : tree) {
        keys.push_back(p.first);
    }
    EXPECT_EQ(keys, std::vector<int>({5, 11, 12, 15, 18}));
    
    for (auto it = tree.begin(); it != tree.end(); it++) {
        auto forward = it;
        forward++;
        if (forward != tree.end()) {
            forward--;
            EXPECT_EQ(forward->first, it->first);
        }
        auto backward = it;
        backward--;
        if (backward != tree.end()) {
            backward++;
            EXPECT_EQ(backward->first, it->first);
        }
    }
}

TEST(BSTTest, CopyAndMove) {
    bst<int, std::string> tree1;
    tree1[1] = "one";
    tree1[2] = "two";
    tree1[3] = "three";
    
    bst<int, std::string> tree2 = tree1;
    EXPECT_EQ(tree2.size(), 3U);
    EXPECT_EQ(tree2[1], "one");
    
    tree2[1] = "modified";
    EXPECT_EQ(tree1[1], "one");
    
    bst<int, std::string> tree3 = std::move(tree1);
    EXPECT_EQ(tree3.size(), 3U);
    EXPECT_TRUE(tree1.empty());
}

TEST(BSTTest, LargeTree) {
    bst<int, int> tree;
    for (int i = 0; i < 1000; i++) {
        tree[i] = i * 2;
    }
    
    EXPECT_EQ(tree.size(), 1000U);
    
    for (int i = 0; i < 1000; i++) {
        EXPECT_TRUE(tree.contains(i));
        EXPECT_EQ(tree[i], i * 2);
    }
}

TEST(BSTTest, ConstIterator) {
    bst<int, std::string> tree;
    tree[1] = "one";
    tree[2] = "two";
    tree[3] = "three";
    
    const auto& ct = tree;
    
    std::vector<int> keys;
    for (auto it = ct.begin(); it != ct.end(); it++) {
        keys.push_back(it->first);
    }
    EXPECT_EQ(keys, std::vector<int>({1, 2, 3}));
}

TEST(BSTTest, Clear) {
    bst<int, std::string> tree;
    tree[1] = "one";
    tree[2] = "two";
    tree[3] = "three";
    
    EXPECT_EQ(tree.size(), 3U);
    tree.clear();
    
    EXPECT_EQ(tree.size(), 0U);
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.begin(), tree.end());
}

TEST(BSTTest, Bug_DecrementFromEmptyEnd) {
    bst<int, std::string> tree;
    auto it = tree.end();
    it--;
    EXPECT_EQ(it, tree.end());
}

TEST(BSTTest, EraseIteratorAndNeighbors) {
    bst<int, std::string> tree;
    tree.insert({2, "two"});
    tree.insert({1, "one"});
    tree.insert({4, "four"});
    tree.insert({3, "three"});
    tree.insert({5, "five"});
    
    auto it = tree.find(3);
    ASSERT_NE(it, tree.end());
    auto prev_it = it;
    prev_it--;
    auto next_it = it;
    next_it++;
    
    tree.erase(it);
    
    EXPECT_EQ(prev_it->first, 2);
    prev_it++;
    EXPECT_EQ(prev_it->first, 4);
    
    EXPECT_EQ(next_it->first, 4);
    next_it--;
    EXPECT_EQ(next_it->first, 2);
    
    for (auto iter = tree.begin(); iter != tree.end(); iter++) {
        auto check = iter;
        check++;
        if (check != tree.end()) {
            check--;
        }
        check--;
        if (check != tree.end()) {
            check++;
        }
    }
}

TEST(BSTTest, CompareWithStdMap) {
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dis(1, 100);
    
    bst<int, int> my_tree;
    std::map<int, int> std_map;
    
    auto check_equal = [&]() {
        EXPECT_EQ(my_tree.size(), std_map.size());
        
        auto my_it = my_tree.begin();
        auto std_it = std_map.begin();
        
        int pos = 0;
        while (my_it != my_tree.end() && std_it != std_map.end()) {
            SCOPED_TRACE("position=" + std::to_string(pos));
            EXPECT_EQ(my_it->first, std_it->first);
            EXPECT_EQ(my_it->second, std_it->second);
            my_it++;
            std_it++;
            pos++;
        }
        
        EXPECT_EQ(my_it, my_tree.end());
        EXPECT_EQ(std_it, std_map.end());
    };
    
    for (int i = 0; i < 100; i++) {
        int key = dis(gen);
        int val = dis(gen) * 10;
        
        SCOPED_TRACE("insert key=" + std::to_string(key));
        my_tree[key] = val;
        std_map[key] = val;
        
        EXPECT_EQ(my_tree.contains(key), std_map.count(key) > 0);
        if (my_tree.contains(key)) {
            EXPECT_EQ(my_tree.find(key)->second, std_map.find(key)->second);
        }
    }
    
    check_equal();
    
    for (int i = 0; i < 30; i++) {
        int key = dis(gen);
        SCOPED_TRACE("erase key=" + std::to_string(key));
        my_tree.erase(key);
        std_map.erase(key);
        
        EXPECT_EQ(my_tree.contains(key), std_map.count(key) > 0);
    }
    
    check_equal();
    
    for (int key = 1; key <= 100; key++) {
        EXPECT_EQ(my_tree.contains(key), std_map.count(key) > 0);
    }
}

TEST(BSTTest, Bug_IteratorConsistencyAfterRandomErase) {
    std::mt19937 gen(123);
    std::uniform_int_distribution<int> dis(1, 50);
    
    bst<int, int> my_tree;
    std::map<int, int> std_map;
    
    for (int i = 1; i <= 50; i++) {
        my_tree[i] = i * 2;
        std_map[i] = i * 2;
    }
    
    for (int i = 0; i < 20; i++) {
        int key = dis(gen);
        my_tree.erase(key);
        std_map.erase(key);
        
        std::vector<int> forward_my, backward_my;
        for (auto it = my_tree.begin(); it != my_tree.end(); it++) {
            forward_my.push_back(it->first);
        }
        for (auto it = my_tree.rbegin(); it != my_tree.rend(); it++) {
            backward_my.push_back(it->first);
        }
        std::reverse(backward_my.begin(), backward_my.end());
        
        EXPECT_EQ(forward_my, backward_my);
        
        auto my_it = my_tree.begin();
        auto std_it = std_map.begin();
        int pos = 0;
        while (my_it != my_tree.end() && std_it != std_map.end()) {
            SCOPED_TRACE("after erase key=" + std::to_string(key) + " pos=" + std::to_string(pos));
            EXPECT_EQ(my_it->first, std_it->first);
            
            auto check_forward = my_it;
            check_forward++;
            if (check_forward != my_tree.end()) {
                check_forward--;
                EXPECT_EQ(check_forward->first, my_it->first);
            }
            
            auto check_backward = my_it;
            check_backward--;
            if (check_backward != my_tree.end()) {
                check_backward++;
                EXPECT_EQ(check_backward->first, my_it->first);
            }
            
            my_it++;
            std_it++;
            pos++;
        }
    }
}

TEST(BSTTest, CopyIndependence) {
    bst<int, std::string> tree1;
    for (int i = 1; i <= 20; i++) {
        tree1[i] = "val" + std::to_string(i);
    }
    
    bst<int, std::string> tree2 = tree1;
    
    std::vector<int> keys1, keys2;
    for (const auto& p : tree1) {
        keys1.push_back(p.first);
    }
    for (const auto& p : tree2) {
        keys2.push_back(p.first);
    }
    EXPECT_EQ(keys1, keys2);
    
    tree1.erase(5);
    tree1.erase(10);
    tree1.erase(15);
    tree1[25] = "new";
    
    tree2.erase(3);
    tree2.erase(7);
    tree2[30] = "other";
    
    keys1.clear();
    keys2.clear();
    for (const auto& p : tree1) {
        keys1.push_back(p.first);
    }
    for (const auto& p : tree2) {
        keys2.push_back(p.first);
    }
    
    EXPECT_NE(keys1, keys2);
    EXPECT_FALSE(std::find(keys1.begin(), keys1.end(), 5) != keys1.end());
    EXPECT_FALSE(std::find(keys1.begin(), keys1.end(), 10) != keys1.end());
    EXPECT_TRUE(std::find(keys1.begin(), keys1.end(), 25) != keys1.end());
    
    EXPECT_FALSE(std::find(keys2.begin(), keys2.end(), 3) != keys2.end());
    EXPECT_FALSE(std::find(keys2.begin(), keys2.end(), 7) != keys2.end());
    EXPECT_TRUE(std::find(keys2.begin(), keys2.end(), 30) != keys2.end());
    
    for (auto it1 = tree1.begin(), it2 = tree2.begin(); 
         it1 != tree1.end() && it2 != tree2.end(); 
         it1++, it2++) {
        auto check1 = it1;
        check1++;
        if (check1 != tree1.end()) {
            check1--;
            EXPECT_EQ(check1->first, it1->first);
        }
        
        auto check2 = it2;
        check2++;
        if (check2 != tree2.end()) {
            check2--;
            EXPECT_EQ(check2->first, it2->first);
        }
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
