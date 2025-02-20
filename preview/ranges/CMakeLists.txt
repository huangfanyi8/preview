cmake_minimum_required(VERSION 3.20)
project(preview.ranges
        VERSION 1.0.0
        DESCRIPTION "c++17实现c++20 std::ranges库"
        LANGUAGES CXX
        HOMEPAGE_URL "https://github.com/huangfanyi8/preview")

enable_testing()

add_library(ranges INTERFACE)

target_sources(ranges INTERFACE ${CMAKE_CURRENT_LIST_DIR}/concepts.hpp)

target_compile_features(ranges INTERFACE 17)

file(GLOB _test_files ${CMAKE_CURRENT_LIST_DIR}/test/*.cpp)

foreach(test_file  ${_test_files})
    GET_FILENAME_COMPONENT(file_name ${test_file} NAME_WE)
    add_executable(${file_name} ${test_file})
    set_target_properties(${file_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/test/bin
            CXX_STANDARD 17)

    target_include_directories(${file_name}
            INTERFACE ${_root_library}/preview)
endforeach ()