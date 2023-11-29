cmake_minimum_required(VERSION 3.10)

# set the project name
project(simple_svg)

# add the executable
add_executable(simple_svg src/main.cpp)

add_executable(simple_svg_tests
                tests/point_test.cpp
                tests/polar_test.cpp
                catch2/catch_amalgamated.cpp)
