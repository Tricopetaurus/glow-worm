
add_library(color_patterns
)

target_link_libraries(
    color_patterns
    color
    pico_stdlib
)

target_include_directories(color_patterns PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})