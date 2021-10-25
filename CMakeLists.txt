cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0048 NEW)
project(easy_reflection VERSION 1.0.0)

enable_testing()

add_subdirectory(library)
add_subdirectory(generator)
add_subdirectory(example)
add_subdirectory(tests)
