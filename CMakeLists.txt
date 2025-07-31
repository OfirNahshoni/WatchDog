# top-level CMakeLists.txt

cmake_minimum_required(VERSION 3.20)
project(WatchDog)

set(CMAKE_C_STANDARD 89)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(test)
