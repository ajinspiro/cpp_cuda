cmake_minimum_required(VERSION 3.10.0)
project(cpp_code VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cpp_code SHARED cpp_code.cpp)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE cpp_code)