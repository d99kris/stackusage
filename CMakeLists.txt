# Project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(stackusage VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -g \
                   -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual \
                   -Wstrict-prototypes -Wmissing-prototypes -Wno-missing-braces -Wswitch-default \
                   -Wcast-align -Wunreachable-code -Wundef -Wuninitialized ")

# Library
add_library(stackusage SHARED src/sumain.c)
install(TARGETS stackusage LIBRARY DESTINATION lib)
set_target_properties(stackusage PROPERTIES LINK_FLAGS "-fPIC")
target_link_libraries(stackusage pthread dl)

# Utility
configure_file(src/stackusage ${CMAKE_CURRENT_BINARY_DIR}/stackusage COPYONLY)
install(PROGRAMS src/stackusage DESTINATION bin)

# Manual
install(FILES src/stackusage.1 DESTINATION share/man/man1)

# Tests
enable_testing()

add_executable(ex001 tests/ex001.c)
target_link_libraries(ex001 pthread)

configure_file(tests/test001 ${CMAKE_CURRENT_BINARY_DIR}/test001 COPYONLY)
add_test(test001 "${PROJECT_BINARY_DIR}/test001")

add_executable(ex002 tests/ex002.c)
target_link_libraries(ex002 pthread)

configure_file(tests/test002 ${CMAKE_CURRENT_BINARY_DIR}/test002 COPYONLY)
add_test(test002 "${PROJECT_BINARY_DIR}/test002")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_executable(ex003 tests/ex003.c)
  target_link_libraries(ex003 pthread)

  configure_file(tests/test003 ${CMAKE_CURRENT_BINARY_DIR}/test003 COPYONLY)
  add_test(test003 "${PROJECT_BINARY_DIR}/test003")
endif()
