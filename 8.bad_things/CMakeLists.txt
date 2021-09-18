cmake_minimum_required(VERSION 3.16)

project(8.bad_things)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

add_executable(${PROJECT_NAME}  main.cpp
                                10.class_exmp.cpp
                                7.pimpl_exmp.cpp
                                5.multi_inheritance_a.cpp
                                5.multi_inheritance_b.cpp
                                5.print_scan.cpp
                                4.poly_a_class.cpp
                                4.poly_b_class.cpp
                                4.non_poly.cpp
                                3.pointers.cpp
                                3.dependency.cpp
                                3.free_functions.cpp
                                1.more_is_better.cpp)

target_link_libraries(${PROJECT_NAME} benchmark)

