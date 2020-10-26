cmake_minimum_required(VERSION 3.18.4)
project(RecordManager LANGUAGES CXX)

add_library(RecordManager ${CMAKE_CURRENT_SOURCE_DIR}/src/RecordManager.cpp)
target_include_directories(RecordManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#target_include_directories(RecordManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../filesystem/bufmanager
#                                         PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../filesystem/fileio
#                                         PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../filesystem/utils)

target_link_libraries(RecordManager Logger)
target_link_libraries(RecordManager FileSystem)
