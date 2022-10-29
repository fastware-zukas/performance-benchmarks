cmake_minimum_required(VERSION 3.16)

project(18.conditionals)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -fno-rtti -fwhole-program")

add_executable(${PROJECT_NAME}  main.cpp)

target_link_libraries(${PROJECT_NAME} benchmark)

