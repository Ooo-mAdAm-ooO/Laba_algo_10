cmake_minimum_required(VERSION 3.25)
project(Rooster LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rooster
    src/main.cpp
)

target_include_directories(rooster PRIVATE src)
