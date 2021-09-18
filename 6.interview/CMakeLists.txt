cmake_minimum_required(VERSION 3.16)

project(6.interview)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} benchmark)

