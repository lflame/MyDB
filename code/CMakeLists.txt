cmake_minimum_required(VERSION 3.18.4)
project(MyDB LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

add_executable(MyDB src/main.cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/recordmanager)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/logger)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/filesystem)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/tests)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/index)
target_link_libraries(MyDB PUBLIC Tests)
target_link_libraries(MyDB PUBLIC Logger)

target_compile_features(MyDB PUBLIC cxx_std_11)