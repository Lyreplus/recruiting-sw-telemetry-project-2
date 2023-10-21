cmake_minimum_required(VERSION 3.21)
project(Project2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread -O0 -g -w")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        # ${CMAKE_CURRENT_SOURCE_DIR}/other/dirs
)

add_library(
        ${PROJECT_NAME}
        SHARED

        # Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/fake_receiver.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/unixTime.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/data_converter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/data_processing.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/state_functions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/thread_functions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/check_statuses.cpp
)

target_link_libraries(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/fake_receiver.c

)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(main
        main.cpp
        src/unixTime.cpp
        src/data_converter.cpp
        src/data_processing.cpp
        src/state_functions.cpp
        src/thread_functions.cpp
        src/check_statuses.cpp
)
target_link_libraries(main ${PROJECT_NAME})