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

