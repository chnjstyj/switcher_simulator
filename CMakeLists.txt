cmake_minimum_required(VERSION 3.27)
project(simulator)

set(CMAKE_CXX_STANDARD 17)
add_executable(simulator src/main.cpp
        #src/sdb.cpp
        src/package.cpp
        src/channel.cpp
        src/mmu.cpp
        src/paddr.cpp
        src/expr.cpp
        src/utils.cpp
)

target_link_libraries(simulator PUBLIC readline)
