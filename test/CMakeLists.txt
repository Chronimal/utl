#########################################################################
# Top level CMakeList including all test harnesses
#########################################################################

cmake_minimum_required(VERSION 3.9.4)

project ("test")

include_directories(../include catch2)

add_subdirectory ("ring_buffer")
add_subdirectory ("temp_buffer")
add_subdirectory ("scope_guard")
