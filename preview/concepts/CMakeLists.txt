cmake_minimum_required(VERSION 3.20)
project(preview.concepts
        VERSION 1.0.0
        DESCRIPTION "c++17实现c++20 concepts"
        LANGUAGES CXX
        HOMEPAGE_URL "https://github.com/huangfanyi8/preview")

enable_testing()

add_library(preview.concepts INTERFACE)

target_sources(preview.concepts INTERFACE ${CMAKE_CURRENT_LIST_DIR}/comparison.hpp
        INTERFACE ${CMAKE_CURRENT_LIST_DIR}/core.hpp
        INTERFACE ${CMAKE_CURRENT_LIST_DIR}/object.hpp)

target_compile_features(preview.concepts INTERFACE 17)
