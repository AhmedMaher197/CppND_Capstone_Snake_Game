cmake_minimum_required(VERSION 3.7)

# Set C++17 as our standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-std=c++17)

# Compiler flags for warnings
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

# Project name
project(SDL2Test)

# Set up the module path for finding packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Find required packages
find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)  # Added this line to find pthread

# Include directories
include_directories(${SDL2_INCLUDE_DIRS} src)

# Define our executable and its source files
add_executable(SnakeGame 
    src/main.cpp 
    src/game.cpp 
    src/controller.cpp 
    src/renderer.cpp 
    src/snake.cpp
)

# Clean up SDL2 libraries string
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)

# Link against our required libraries
target_link_libraries(SnakeGame 
    ${SDL2_LIBRARIES}
    Threads::Threads  # Added this line to link against pthread
)