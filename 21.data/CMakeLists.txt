cmake_minimum_required(VERSION 3.16)

project(21.data)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

include_directories(.)

add_executable(${PROJECT_NAME} main.cpp Baseline.cpp)

target_link_libraries(${PROJECT_NAME} benchmark)

