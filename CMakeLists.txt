project(StrongTypedef)
cmake_minimum_required(VERSION 2.8.12)

add_definitions("-std=c++17")
include_directories(. ./src)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(StrongTypedefTest src/strong_typedef_test.cpp)
target_link_libraries(StrongTypedefTest ${CONAN_LIBS_GTEST})

enable_testing()

add_test(NAME StrongTypedef
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
         COMMAND StrongTypedefTest)
