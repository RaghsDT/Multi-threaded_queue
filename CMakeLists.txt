cmake_minimum_required(VERSION 3.10)
project(ThreadSafeQueue)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Define the queue library
add_library(Queue Queue.h Queue.tpp)

# Define the test executable
add_executable(QueueTests TestQueue.cpp)
target_link_libraries(QueueTests Queue GTest::GTest GTest::Main pthread)

enable_testing()
add_test(NAME QueueTests COMMAND QueueTests)

