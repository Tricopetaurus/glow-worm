
add_library(leds
    leds.cpp
)

target_link_libraries(
    leds
    color_patterns
    neopixel
    pico_stdlib
    hardware_gpio
)

add_subdirectory(color)
add_subdirectory(neopixel)
add_subdirectory(color_patterns)
target_include_directories(leds PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})