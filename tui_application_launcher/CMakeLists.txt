cmake_minimum_required (VERSION 3.11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
 
# fetch ftxui
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# add boost
# set(Boost_INCLUDE_DIR /usr/include)
# set(Boost_LIBRARY_DIR /usr/lib64)

# find_package(Boost 1.81.0 COMPONENTS filesystem REQUIRED)

file(GLOB SOURCES src/*.cpp)
 
# set bin dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set project name
project(tui_app_launcher LANGUAGES CXX VERSION 1.0.0)

add_executable(${PROJECT_NAME} ${SOURCES})


find_package(Boost 1.81 REQUIRED COMPONENTS filesystem)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS} include)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
    PRIVATE ${Boost_LIBRARIES}
  )
else()
  message(FATAL_ERROR "Could not find Boost")
endif()

message(STATUS "boost libraries: " ${Boost_LIBRARIES})
