cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE _test_files ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

foreach(test_file  ${_test_files})
    GET_FILENAME_COMPONENT(file_name ${test_file} NAME_WE)
    add_executable(${file_name} ${test_file})
    set_target_properties(${file_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
            CXX_STANDARD 17)

    target_include_directories(${file_name}
            INTERFACE ${_root_library}/preview)
endforeach ()