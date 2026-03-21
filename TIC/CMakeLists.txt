cmake_minimum_required(VERSION 3.10)
project(TIC)

add_subdirectory(googletest)

add_executable(test_tic test_tic.cpp tic.cpp)
target_link_libraries(test_tic gtest)

add_executable(tic_app main.cpp tic.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")