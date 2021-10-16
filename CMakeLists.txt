cmake_minimum_required(VERSION 3.10)
project(easy_reflection VERSION 0.1.0)

add_subdirectory(library)
add_subdirectory(generator)
add_subdirectory(example)
add_subdirectory(tests)

enable_testing()