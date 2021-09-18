cmake_minimum_required(VERSION 3.16)

project(15.sync)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

add_executable(${PROJECT_NAME}  main.cpp)

target_link_libraries(${PROJECT_NAME} benchmark)

