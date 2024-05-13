cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cell_program VERSION 1.0)
SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall")
add_executable(${PROJECT_NAME} src/main.cpp src/CellGrid.cpp)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
