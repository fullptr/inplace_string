cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_executable(
    tests
    basic.cpp
    iterators.cpp
)

enable_testing()
find_package(GTest CONFIG REQUIRED)
target_link_libraries(
    tests
    GTest::gtest_main
)

target_include_directories(tests PRIVATE ../include)