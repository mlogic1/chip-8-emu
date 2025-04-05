cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Chip8Emu)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)

add_executable(chip8emu 	chip8/main.cpp
							chip8/chip8.cpp
							chip8/EmuWindow.cpp)

target_include_directories(chip8emu PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(chip8emu PRIVATE ${SDL2_LIBRARIES})