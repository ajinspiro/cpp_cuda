cmake_minimum_required(VERSION 3.10.0)
project(cpp_graphics VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpp_graphics main.cpp)
add_executable(MFCClock WIN32 MFCClock.cpp)
target_compile_definitions(MFCClock PRIVATE _AFXDLL)