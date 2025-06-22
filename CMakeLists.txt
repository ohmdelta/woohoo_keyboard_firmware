cmake_minimum_required(VERSION 3.13...3.27)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()
project(firmware)

add_executable(firmware)

file(MAKE_DIRECTORY generated)
pico_generate_pio_header(firmware ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(firmware PUBLIC pico_stdlib hardware_pio pico_unique_id tinyusb_device tinyusb_board)

target_sources(firmware PUBLIC
  src/main.c
  src/usb_descriptors.c
  src/sym_defer_pk.c
  src/firmware_timer.c
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(firmware)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(firmware PUBLIC
  include
)
#
#
# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
#target_compile_definitions(firmware PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)

pico_add_extra_outputs(firmware)

