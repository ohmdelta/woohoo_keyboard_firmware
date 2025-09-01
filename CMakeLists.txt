cmake_minimum_required(VERSION 3.13...3.27)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()
project(firmware)

file(MAKE_DIRECTORY generated)

add_subdirectory(ssd1306)

function (setup_side FIRMWARE)
  add_executable(${FIRMWARE})
  pico_generate_pio_header(${FIRMWARE} ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

  # Add pico_stdlib library which aggregates commonly used features
  target_link_libraries(${FIRMWARE} PUBLIC pico_stdlib pico_multicore hardware_pio pico_unique_id tinyusb_device tinyusb_board ssd1306)
  target_compile_options(${FIRMWARE} PRIVATE -Wall -Wextra -Werror -O10)

  set_target_properties(${FIRMWARE} PROPERTIES COMPILE_DEFINITIONS "PICO_PLATFORM=rp2350 PICO_BOARD=pico2_xl PICO_UART_ENABLE_CRLF_SUPPORT=0")

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
    src/ui/ui.c
    src/ui/timer.c
    src/ui/common.c
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
set_target_properties(right_firmware PROPERTIES COMPILE_DEFINITIONS "KEYBOARD_SIDE=1")
