cmake_minimum_required(VERSION 3.28)
project(Binary-Tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Binary-Tree INTERFACE)

target_include_directories(Binary-Tree INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
