cmake_minimum_required(VERSION 3.18.4)
project(FileSystem LANGUAGES CXX)

add_library(FileSystem ${CMAKE_CURRENT_SOURCE_DIR}/src/MyBitMap.cpp)

target_include_directories(FileSystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/bufmanager
                                      PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/fileio
                                      PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/utils)
