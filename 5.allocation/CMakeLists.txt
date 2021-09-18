cmake_minimum_required(VERSION 3.16)

project(5.allocation)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsized-deallocation")


add_executable(${PROJECT_NAME} "main.cpp" "alloc.cpp")
target_link_libraries(${PROJECT_NAME} benchmark)

