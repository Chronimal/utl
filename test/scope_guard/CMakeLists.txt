#########################################################################
# Test harnesses for ScopeGuard
#########################################################################

cmake_minimum_required(VERSION 3.9.4)
project (catch_scope_guard CXX)

add_executable(${PROJECT_NAME} "catch_scope_guard.cpp")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ../catch2 ../../include)
