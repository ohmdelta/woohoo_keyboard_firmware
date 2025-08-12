project(ssd1306)

include_directories(lib)

add_library(
        ssd1306
        src/util.c
)

target_include_directories(
        ssd1306
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        lib
)

# pull in common dependencies and additional i2c hardware support
target_link_libraries(ssd1306 pico_stdlib hardware_i2c)

add_executable(
        ssd1306_main
        src/main.c
)
target_link_libraries(ssd1306_main ssd1306 pico_stdlib hardware_i2c)
# create map/bin/hex file etc.
pico_add_extra_outputs(ssd1306_main)
