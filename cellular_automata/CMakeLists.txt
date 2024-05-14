cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cell_program VERSION 1.0)
SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall")

# include header files
include_directories(include)

# glob all .cpp files in source directory to sources variable
file(GLOB SOURCES src/*.cpp)
# add executable based on project name, compiling the soures in src/*.cpp
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
