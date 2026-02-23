cmake_minimum_required(VERSION 4.1)
project(af_memory C)

set(CMAKE_C_STANDARD 17)

add_executable(af_memory)

target_include_directories(af_memory PRIVATE include)
target_sources(af_memory PRIVATE
        src/main.c
        src/memory_manager.c
        src/seg_free_list.c
)
