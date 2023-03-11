cmake_minimum_required(VERSION 3.24)
project(ALGOTrading)

set(CMAKE_CXX_STANDARD 17)

find_package(DataFrame REQUIRED)

add_executable(ALGOTrading main.cpp DataProcessing/DataSource.h)

target_link_libraries(ALGOTrading DataFrame::DataFrame)