
add_executable(glowworm
    main.cpp
)

pico_enable_stdio_usb(glowworm 1)

pico_add_extra_outputs(glowworm)

add_subdirectory(leds)
add_subdirectory(usb)
add_subdirectory(accelerometer)

target_link_libraries(
    glowworm
    leds
    usb
    accelerometer
    pico_stdlib
    hardware_adc
    hardware_dma
    hardware_pio
    hardware_sync
    pico_multicore
)
