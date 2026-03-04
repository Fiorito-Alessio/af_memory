cmake_minimum_required(VERSION 3.25)
project(af_memory C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(af_memory SHARED)

target_include_directories(af_memory PUBLIC include)

target_sources(af_memory PRIVATE
        src/main.c
        src/memory_manager.c
        src/seg_free_list.c
        src/splitter.c
        src/af_malloc.c
        src/af_free.c
        src/af_calloc.c
        src/af_realloc.c
)

find_package(Threads REQUIRED)
target_link_libraries(af_memory PRIVATE Threads::Threads)

target_compile_options(af_memory PRIVATE
        -Wall
        -Wextra
        -Wshadow
        -Wconversion
        -Werror
        -g
        -fsanitize=undefined
        -fno-omit-frame-pointer
)

target_link_options(af_memory PRIVATE
        -fsanitize=undefined
)

install(TARGETS af_memory DESTINATION lib)
install(FILES include/af_memory.h DESTINATION include)