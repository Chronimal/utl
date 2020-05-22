#########################################################################
# Test harnesses for ScopeGuard
#########################################################################

cmake_minimum_required(VERSION 3.9.4)

add_executable(catch_scope_guard "catch_scope_guard.cpp")
target_compile_features(catch_scope_guard PRIVATE cxx_std_20)
