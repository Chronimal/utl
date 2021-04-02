#########################################################################
# Test harnesses for TempBuffer<L>
#########################################################################

cmake_minimum_required(VERSION 3.9.4)
project (catch_temp_buffer CXX)

add_executable(${PROJECT_NAME} "catch_temp_buffer.cpp")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ../catch2 ../../include)
