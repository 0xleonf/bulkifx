cmake_minimum_required(VERSION 3.10)
project(ImageProcessor C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

set(LIB_SOURCES
    src/core.c
    src/img_proc.c
    src/bulk.c
)

add_library(bulkifx SHARED ${LIB_SOURCES})

target_link_libraries(bulkifx m)

add_executable(bulkifxApp main.c)
target_link_libraries(bulkifxApp bulkifx)

