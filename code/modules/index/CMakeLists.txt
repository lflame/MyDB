cmake_minimum_required(VERSION 3.18.4)
project(Index LANGUAGES CXX)

add_library(Index ${CMAKE_CURRENT_SOURCE_DIR}/src/IndexManager.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/src/BPlusTree.cpp)
target_include_directories(Index PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Index Logger)
target_link_libraries(Index FileSystem)
target_link_libraries(Index RecordManager)
