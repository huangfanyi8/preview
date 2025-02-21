cmake_minimum_required(VERSION 3.20)
project(preview.ranges
        VERSION 1.0.0
        DESCRIPTION "c++17实现c++20 std::ranges库"
        LANGUAGES CXX
        HOMEPAGE_URL "https://github.com/huangfanyi8/preview")

enable_testing()

add_library(preview.ranges INTERFACE)

target_sources(preview.ranges INTERFACE ${CMAKE_CURRENT_LIST_DIR}/iter_move.hpp
        INTERFACE ${CMAKE_CURRENT_LIST_DIR}/predicate.hpp
        INTERFACE ${CMAKE_CURRENT_LIST_DIR}/swap.hpp)

target_compile_features(preview.ranges INTERFACE 17)

