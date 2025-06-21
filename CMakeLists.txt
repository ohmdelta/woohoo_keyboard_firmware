cmake_minimum_required(VERSION 3.13...3.27)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(firmware)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(firmware
    src/main.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(firmware PUBLIC pico_stdlib pico_unique_id tinyusb_device tinyusb_board)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(firmware)

target_sources(firmware PUBLIC
  src/main.c
  src/usb_descriptors.c
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(firmware PUBLIC
  include
)
#
#
# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
#target_compile_definitions(firmware PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)

pico_add_extra_outputs(firmware)

