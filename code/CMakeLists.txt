cmake_minimum_required(VERSION 3.18.4)
project(MyDB LANGUAGES CXX)

add_executable(MyDB src/main.cpp)
target_include_directories(MyDB PUBLIC ${CMAKE_SOURCE_DIR}/src/filesystem/bufmanager
                                PUBLIC ${CMAKE_SOURCE_DIR}/src/filesystem/fileio
                                PUBLIC ${CMAKE_SOURCE_DIR}/src/filesystem/utils)

