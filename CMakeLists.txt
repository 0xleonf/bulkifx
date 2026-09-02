cmake_minimum_required(VERSION 3.10)
project(bulkifx C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(CTest)
include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.c")

add_library(bulkifx_static STATIC ${SOURCES})
target_compile_options(bulkifx_static PRIVATE -Wall -Wextra)

if(BUILD_TESTING)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(CHECK REQUIRED check)
  find_package(Threads REQUIRED)

  foreach(algo IN ITEMS grayscale sepia)
    add_executable(test_${algo} test/test_${algo}.c)
    target_compile_options(test_${algo} PRIVATE ${CHECK_CFLAGS_OTHER})
    target_include_directories(test_${algo} PRIVATE ${CHECK_INCLUDE_DIRS})
    target_link_directories(test_${algo} PRIVATE ${CHECK_LIBRARY_DIRS})
    target_link_libraries(test_${algo} PRIVATE ${CHECK_LIBRARIES} bulkifx_static Threads::Threads)

    add_test(NAME ${algo} COMMAND test_${algo})
  endforeach()
endif()
