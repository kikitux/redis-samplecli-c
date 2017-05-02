cmake_minimum_required(VERSION 3.5)
project(cli)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES main.c)

if (NOT APPLE)
    FIND_PACKAGE( OpenMP REQUIRED)
    if(OPENMP_FOUND)
        message("OPENMP FOUND")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    endif()
endif()

if (APPLE)
    # brew instal hiredis
    # brew install gcc --without-multilib
    include_directories("/usr/local//Cellar/hiredis/0.13.3/include/hiredis")
    include_directories("/usr/local/Cellar/gcc/6.3.0_1/lib/gcc/6/gcc/x86_64-apple-darwin16.5.0/6.3.0/include")
else()
    include_directories("/usr/include/hiredis")
    include_directories("/usr/lib/gcc/x86_64-linux-gnu/7/include")
endif()

add_executable(cli ${SOURCE_FILES})
target_link_libraries(cli -lhiredis)
