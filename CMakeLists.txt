cmake_minimum_required(VERSION 3.5)
project(Elfy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(third_party/dbg-macro)
include_directories(third_party/cpp_demangle/c_api)

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

include(ExternalProject)

set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/Rust)

ExternalProject_Add(
  cpp_demangle_c_api
  DOWNLOAD_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND cargo build COMMAND cargo build --release
  BINARY_DIR "${CMAKE_SOURCE_DIR}/third_party/cpp_demangle/c_api"
  INSTALL_COMMAND ""
  LOG_BUILD ON
)
