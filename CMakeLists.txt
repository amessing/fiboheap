cmake_minimum_required(VERSION 3.12...3.18)
message("Building fiboheap")
project(fiboheap CXX)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(build_tests ON CACHE STRING "Build Unit Tests")
else(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(build_tests OFF)
endif (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} 
    INTERFACE
        include/fiboheap/fibo_node.hpp
        include/fiboheap/fibo_heap.hpp
        include/fiboheap/fibo_queue.hpp)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_link_libraries(${PROJECT_NAME} INTERFACE robin_hood)

target_include_directories(${PROJECT_NAME} 
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

if(build_tests)
    add_executable(${PROJECT_NAME}_test)
    target_sources(${PROJECT_NAME}_test PUBLIC test/test_fiboheap.cc)
    target_compile_features(${PROJECT_NAME}_test PUBLIC cxx_std_17)
    target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
endif(build_tests)