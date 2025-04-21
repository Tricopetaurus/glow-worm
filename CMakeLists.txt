cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(PICO_SDK_FETCH_FROM_GIT ON)
set(PICO_BOARD adafruit_feather_rp2040 CACHE STRING "Board type")

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

pico_sdk_init()

project(GLOW_WORM C CXX ASM)
add_subdirectory(src)
