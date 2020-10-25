cmake_minimum_required(VERSION 3.18.4)
project(Logger LANGUAGES CXX)

add_library(Logger ${CMAKE_CURRENT_SOURCE_DIR}/src/Logger.cpp)
target_include_directories(Logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
