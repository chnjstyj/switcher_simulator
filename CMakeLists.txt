cmake_minimum_required(VERSION 3.27)
project(simulator)

set(CMAKE_CXX_STANDARD 17)
add_executable(simulator main.cpp
        com.cpp)

target_link_libraries(simulator PUBLIC readline)
