cmake_minimum_required(VERSION 3.24)
project(ALGOTrading)

set(CMAKE_CXX_STANDARD 20)

find_package(DataFrame REQUIRED)

add_executable(ALGOTrading main.cpp DataProcessing/DataSource.h DataProcessing/DataPoints.h DataProcessing/TiingoApi.cpp)

target_link_libraries(ALGOTrading DataFrame::DataFrame)