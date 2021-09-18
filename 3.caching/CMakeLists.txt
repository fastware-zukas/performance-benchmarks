cmake_minimum_required(VERSION 3.0.0)
project(3.caching VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(${PROJECT_NAME} main.cpp part_main.cpp)
target_link_libraries(${PROJECT_NAME} benchmark)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
