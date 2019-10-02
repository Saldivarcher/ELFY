cmake_minimum_required(VERSION 3.5)
project(Elfy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(third_party/dbg-macro)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Add clang sanitizers...")
    set(_ELFY_CLANG_SANITIZERS
        -fsanitize=address -fsanitize=undefined)
    add_compile_options(${_ELFY_CLANG_SANITIZERS})
    link_libraries(${_ELFY_CLANG_SANITIZERS})
  endif()
endif()

add_subdirectory(src)

