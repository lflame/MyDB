cmake_minimum_required(VERSION 3.18.4)
project(RecordManager LANGUAGES CXX)

add_library(RecordManager ${CMAKE_CURRENT_SOURCE_DIR}/src/RecordManager.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/AttrType.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/BufferReader.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/BufferWriter.cpp)
target_include_directories(RecordManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(RecordManager Logger)
target_link_libraries(RecordManager FileSystem)
