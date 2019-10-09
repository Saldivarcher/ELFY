set(SOURCE_FILES main.cc elf.cc utility.cc)
add_executable(elfy ${SOURCE_FILES})
#Link against files in here in the future, if need be
install(TARGETS elfy RUNTIME DESTINATION bin)
