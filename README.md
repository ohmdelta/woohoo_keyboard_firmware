# WOOHOO Keyboard!

This is firmware for custom split keyboard based on based on RP2350B supporting directly wired keys, encoder and optional i2c screen.
The firmware is largely based on the pico-sdk, tiny usb and qmk firmware.


## Schematic:

The keyboard uses RP2350B as the microcontroller - directly connecting to keys:

![Left Keyboard schematic](doc/pcb_layout/left/split_keyboard.svg)


### Left Board Rendering:
![Left Board Rendering](doc/img/left_rendering.png)


### Right Board Rendering:
![Right Board Rendering](doc/img/right_rendering.png)



## Build

Requirements:
* cmake
* gcc arm compiler: `arm-none-eabi-gcc`

```bash
git submodule update --init --recursive
mkdir build
cd build
cmake .. -DPICO_BOARD=pico2_xl -DPICO_PLATFORM=rp2350
make
```
