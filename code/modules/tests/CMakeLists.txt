cmake_minimum_required(VERSION 3.18.4)
project(Tests LANGUAGES CXX)

add_library(Tests ${CMAKE_CURRENT_SOURCE_DIR}/src/Tests.cpp)
target_include_directories(RecordManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Tests Logger)
target_link_libraries(Tests FileSystem)
target_link_libraries(Tests RecordManager)
