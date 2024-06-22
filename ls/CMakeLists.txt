cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(DCMAKE_EXPORT_COMPILE_COMMANDS ON)


project(pls VERSION 1.0)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall")

# include header files
include_directories(include)

# glob all .cpp files in source directory to sources variable
file(GLOB SOURCES src/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# add executable based on project name, compiling the soures in src/*.cpp
add_executable(${PROJECT_NAME} ${SOURCES})


# unit tests
option(BUILD_TESTS "BUILD_TESTS" OFF)
if(BUILD_TESTS)
    message("building tests")
    find_package(Catch2 2 REQUIRED)

    list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)
    file(GLOB TEST_SOURCES tests/*.cpp)
    add_executable(tests ${TEST_SOURCES} ${SOURCES})
    target_link_libraries(tests Catch2::Catch2)
else()
    message("skipping test building")
endif()

