
add_library(accelerometer
    lis3dh.h
    accelerometer.cpp
    accelerometer.h
)

target_link_libraries(
        accelerometer
        pico_stdlib
        hardware_i2c
        hardware_gpio
)

target_include_directories(accelerometer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})