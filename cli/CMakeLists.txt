cmake_minimum_required(VERSION 3.7)
project(cli)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c)

if (APPLE)
    include_directories("/usr/local//Cellar/hiredis/0.13.3/include/hiredis")
else()
    include_directories("/usr/include/hiredis")
endif()

add_executable(cli ${SOURCE_FILES})

if (NOT APPLE)
    target_link_libraries(cli -lhiredis)
endif()