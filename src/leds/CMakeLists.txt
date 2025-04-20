
add_library(leds
    leds.cpp
)

target_link_libraries(
    leds
    neopixel
    color
    pico_stdlib
    hardware_gpio
)

add_subdirectory(color)
add_subdirectory(neopixel)
target_include_directories(leds PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})