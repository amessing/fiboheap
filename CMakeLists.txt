cmake_minimum_required(VERSION 3.12...3.18)
message("Building fiboheap")
project(fiboheap CXX)

add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE src)

add_executable(${PROJECT_NAME}_test)
target_sources(${PROJECT_NAME}_test PUBLIC test/test_fiboheap.cc)
target_compile_features(${PROJECT_NAME}_test PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
