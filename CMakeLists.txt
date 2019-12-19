cmake_minimum_required(VERSION 3.15)
project(Chomp)

set(CMAKE_CXX_STANDARD 20)

include_directories("src/public")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data)

add_executable(Chomp
    src/private/main.cpp
    src/private/gui.cpp
    src/private/robot.cpp
    src/private/player.cpp)