cmake_minimum_required(VERSION 3.28.3)
project(zsl C CXX)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -Wall -Wextra -Wpedantic -Werror")

option(BUILD_TESTS "Build tests")

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(GEN_DIR "${CMAKE_SOURCE_DIR}/gen")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/test")

file(GLOB_RECURSE TEST_SOURCE ${TEST_DIR}/test_main.c ${SRC_DIR}/*.c ${SRC_DIR}/*.h ${GEN_DIR}/*.c ${GEN_DIR}/*.h)
add_executable(ztest ${TEST_SOURCE})
target_include_directories(ztest PUBLIC ${SRC_DIR} ${GEN_DIR})
