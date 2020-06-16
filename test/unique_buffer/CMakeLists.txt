#########################################################################
# Test harnesses for UniqueBuffer
#########################################################################

cmake_minimum_required(VERSION 3.9.4)

add_executable(catch_unique_buffer "catch_unique_buffer.cpp")
target_compile_features(catch_unique_buffer PRIVATE cxx_std_20)
