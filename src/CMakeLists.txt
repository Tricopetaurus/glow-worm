
add_executable(glowworm
    main.cpp
)

pico_enable_stdio_usb(glowworm 1)
pico_enable_stdio_uart(glowworm 0)

pico_add_extra_outputs(glowworm)

add_subdirectory(neopixel)
add_subdirectory(color)

target_link_libraries(
    glowworm
    neopixel
    color
    pico_stdlib
    hardware_adc
    hardware_dma
    hardware_pio
    hardware_sync
    pico_multicore
)
