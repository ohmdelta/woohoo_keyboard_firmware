cmake_minimum_required(VERSION 3.13...3.27)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()
project(firmware)

file(MAKE_DIRECTORY generated)

function (setup_side FIRMWARE)
  add_executable(${FIRMWARE})
  pico_generate_pio_header(${FIRMWARE} ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

  # Add pico_stdlib library which aggregates commonly used features
  target_link_libraries(${FIRMWARE} PUBLIC pico_stdlib hardware_pio pico_unique_id tinyusb_device tinyusb_board)
  target_compile_options(${FIRMWARE} PRIVATE -Wall -Wextra -Werror)

  target_sources(${FIRMWARE} PUBLIC
    src/main.c
    src/matrix.c
    src/board.c
    src/usb_descriptors.c
    src/sym_defer_pk.c
    src/keycode_buffer.c
    src/keyboard_led.c
    src/firmware_timer.c
    src/layer.c
  )

  # create map/bin/hex/uf2 file in addition to ELF.
  pico_add_extra_outputs(${FIRMWARE})

  # Make sure TinyUSB can find tusb_config.h
  target_include_directories(${FIRMWARE} PUBLIC
    include
  )
endfunction()

setup_side(left_firmware)
setup_side(right_firmware)

add_executable(ssd1306_i2c
        ssd1306_i2c/ssd1306_i2c.c
)
target_include_directories(ssd1306_i2c PUBLIC
  ssd1306_i2c
)
target_link_libraries(ssd1306_i2c pico_stdlib hardware_i2c)

pico_add_extra_outputs(ssd1306_i2c)
