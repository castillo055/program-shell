cmake_minimum_required(VERSION 3.13)
project(program_shell CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(SYSTEM include)
add_subdirectory(src)
add_subdirectory(tests)
