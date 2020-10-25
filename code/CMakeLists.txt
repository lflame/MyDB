cmake_minimum_required(VERSION 3.18.4)
project(MyDB LANGUAGES CXX)

add_executable(MyDB src/main.cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/recordmanager)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/logger)
target_link_libraries(MyDB RecordManager)
