cmake_minimum_required(VERSION 3.13...3.27)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()
project(firmware)

add_executable(left_firmware)

file(MAKE_DIRECTORY generated)
pico_generate_pio_header(left_firmware ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(left_firmware PUBLIC pico_stdlib hardware_pio pico_unique_id tinyusb_device tinyusb_board)

target_sources(left_firmware PUBLIC
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
pico_add_extra_outputs(left_firmware)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(left_firmware PUBLIC
  include
)
#
#
# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
#target_compile_definitions(firmware PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)

# pico_add_extra_outputs(firmware)


add_executable(right_firmware)

pico_generate_pio_header(right_firmware ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)
# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(right_firmware PUBLIC pico_stdlib hardware_pio pico_unique_id tinyusb_device tinyusb_board)
set_target_properties(right_firmware PROPERTIES COMPILE_DEFINITIONS "KEYBOARD_SIDE=1")
target_sources(right_firmware PUBLIC
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
pico_add_extra_outputs(right_firmware)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(right_firmware PUBLIC
  include
)
pico_add_extra_outputs(right_firmware)
