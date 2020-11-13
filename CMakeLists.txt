cmake_minimum_required(VERSION 3.12...3.18)
message("Building fiboheap")
project(fiboheap CXX)


# Determine if this project is built as a subproject (using add_subdirectory)
# or if it is the master project.
set(MASTER_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MASTER_PROJECT ON)
    message(STATUS "CMake version: ${CMAKE_VERSION}")
endif (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

# TODO: add documentation and coverage?
if(NOT DEFINED build_tests)
    option(build_tests "Build Unit Tests" ${MASTER_PROJECT})
else(NOT DEFINED build_tests)
    set(build_tests ${MASTER_PROJECT})
endif(NOT DEFINED build_tests)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
        src/fiboheap/fibo_node.hpp src/fiboheap/fibo_heap.hpp src/fiboheap/fibo_queue.hpp)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_link_libraries(${PROJECT_NAME} INTERFACE robin_hood)
target_include_directories(${PROJECT_NAME} INTERFACE src/)

if(build_tests)
    add_executable(${PROJECT_NAME}_test)
    target_sources(${PROJECT_NAME}_test PUBLIC test/test_fiboheap.cc)
    target_compile_features(${PROJECT_NAME}_test PUBLIC cxx_std_17)
    target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
endif(build_tests)