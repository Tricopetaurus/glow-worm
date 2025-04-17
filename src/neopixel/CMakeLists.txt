
add_library(neopixel
    NeoPixelConnect.cpp
)

target_link_libraries(
    neopixel
    pico_stdlib
    hardware_pio
    hardware_dma
)

target_include_directories(neopixel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})