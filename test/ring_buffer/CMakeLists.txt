#########################################################################
# Test harnesses for ring buffer
#########################################################################

cmake_minimum_required(VERSION 3.9.4)

add_executable(catch_ring_buffer "catch_ring_buffer.cpp")

target_compile_features(catch_ring_buffer PRIVATE cxx_std_17)

if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    target_compile_options(catch_ring_buffer PRIVATE "/Zc:__cplusplus")
endif()
