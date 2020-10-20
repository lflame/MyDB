CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)
PROJECT(MyDB)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

# add_compile_options(-g)
# add_compile_options(-O0)
add_compile_options(-O3)


SET(MyDB_SOURCES
        src/image.cpp
        src/main.cpp
        src/mesh.cpp
        src/scene_parser.cpp
        src/utils.cpp
        )


SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

ADD_EXECUTABLE(${PROJECT_NAME} ${MyDB_SOURCES})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)
