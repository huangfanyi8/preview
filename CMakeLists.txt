cmake_minimum_required(VERSION 3.20)
project(preview DESCRIPTION "c++实现自己的工具库"
                            LANGUAGES CXX)

set(_root_library ${CMAKE_CURRENT_LIST_DIR})
add_subdirectory(preview/meta)
add_subdirectory(preview/ranges)