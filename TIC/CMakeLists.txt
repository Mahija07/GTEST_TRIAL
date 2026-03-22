cmake_minimum_required(VERSION 3.10)
project(TIC)

add_subdirectory(googletest)

add_executable(test_tic test_tic.cpp tic.cpp)
target_link_libraries(test_tic gtest)

add_executable(tic_app main.cpp tic.cpp)

add_library(tic SHARED tic.cpp tic_c_api.cpp)
# export symbols for python ctypes
if (MSVC)
    target_compile_definitions(tic PRIVATE TIC_EXPORTS)
endif()

set_target_properties(tic PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")