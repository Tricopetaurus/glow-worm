
add_library(usb
    usb.cpp
)

target_link_libraries(
    usb
    pico_stdlib
    hardware_uart
)

pico_enable_stdio_usb(usb 1)
target_include_directories(usb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})