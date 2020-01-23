set(SOURCE_FILES main.cc elf.cc utility.cc)
add_executable(elfy ${SOURCE_FILES})

#Link against files in here in the future, if need be
add_dependencies(elfy cpp_demangle_c_api)
target_link_libraries(elfy
  debug "${CMAKE_SOURCE_DIR}/third_party/cpp_demangle/c_api/target/debug/libcpp_demangle_c_api.so"
)

install(TARGETS elfy RUNTIME DESTINATION bin)

