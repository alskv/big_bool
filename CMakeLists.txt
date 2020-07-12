cmake_minimum_required(VERSION 3.14)

project(big_bool C)

set(CMAKE_CSTANDARD 99)

set(SOURCE_EXE test.c)

set(SOURCE_LIB vectorlib.c)

add_library(vectorlib STATIC ${SOURCE_LIB})

add_executable(test ${SOURCE_EXE})

target_link_libraries(test vectorlib)
