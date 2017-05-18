# Project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(stackusage VERSION 1.0 LANGUAGES C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -D_XOPEN_SOURCE=600 \
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
add_executable(sutest01 tests/sutest01.c)
target_link_libraries(sutest01 pthread)
add_executable(sutest02 tests/sutest02.c)
target_link_libraries(sutest02 pthread)

