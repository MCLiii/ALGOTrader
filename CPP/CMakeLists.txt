set(CMAKE_AUTOMOC ON)

cmake_minimum_required(VERSION 3.24)
project(ALGOTrading)

set(CMAKE_CXX_STANDARD 20)

find_package(DataFrame REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Network)

add_executable(ALGOTrading main.cpp DataProcessing/DataSource.h DataProcessing/DataPoints.h DataProcessing/TiingoApi.cpp DataProcessing/test.cpp)

target_link_libraries(ALGOTrading PRIVATE DataFrame::DataFrame Qt6::Network)