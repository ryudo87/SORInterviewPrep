cmake_minimum_required(VERSION 3.15)

project(HelloWorld VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(HelloWorld src/main.cpp)

target_compile_options(HelloWorld PRIVATE -O3 -g -fno-omit-frame-pointer)
target_link_libraries(HelloWorld PRIVATE pthread)
