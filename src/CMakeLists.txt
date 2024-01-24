
add_executable(rp2040_led
    main.cpp
    NeoPixelConnect.cpp
)

pico_enable_stdio_usb(rp2040_led 1)
pico_enable_stdio_uart(rp2040_led 0)

pico_add_extra_outputs(rp2040_led)

target_link_libraries(
    rp2040_led
    pico_stdlib
    hardware_adc
    hardware_dma
    hardware_pio
    hardware_sync
    pico_multicore
)
