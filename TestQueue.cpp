#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "Queue.h"

// Test case: Basic push/pop functionality
TEST(QueueTest, PushPop) {
    Queue<int> q(3);
    q.Push(1);
    q.Push(2);
    q.Push(3);

    EXPECT_EQ(q.Pop(), 1);
    EXPECT_EQ(q.Pop(), 2);
    EXPECT_EQ(q.Pop(), 3);
}

// Test case: Push beyond capacity (oldest element removed)
TEST(QueueTest, PushBeyondCapacity) {
    Queue<int> q(3);
    q.Push(1);
    q.Push(2);
    q.Push(3);
    q.Push(4);  // This should remove 1

    EXPECT_EQ(q.Pop(), 2);
    EXPECT_EQ(q.Pop(), 3);
    EXPECT_EQ(q.Pop(), 4);
}

// Test case: Blocking Pop
TEST(QueueTest, BlockingPop) {
    Queue<int> q(2);
    std::thread producer([&q] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        q.Push(10);
        });

    EXPECT_EQ(q.Pop(), 10);
    producer.join();
}

// Test case: Timeout Pop
TEST(QueueTest, PopWithTimeout) {
    Queue<int> q(2);
    EXPECT_THROW(q.PopWithTimeout(100), std::runtime_error);

    q.Push(5);
    EXPECT_EQ(q.PopWithTimeout(100), 5);
}

// Test case: Count and Size
TEST(QueueTest, CountAndSize) {
    Queue<int> q(5);
    EXPECT_EQ(q.Size(), 5);
    EXPECT_EQ(q.Count(), 0);

    q.Push(1);
    q.Push(2);
    EXPECT_EQ(q.Count(), 2);
}