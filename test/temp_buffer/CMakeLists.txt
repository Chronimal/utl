#########################################################################
# Test harnesses for TempBuffer<L>
#########################################################################

cmake_minimum_required(VERSION 3.9.4)

add_executable(catch_temp_buffer "catch_temp_buffer.cpp")
target_compile_features(catch_temp_buffer PRIVATE cxx_std_20)
