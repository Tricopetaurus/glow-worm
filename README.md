# Glow Worm

## Overview


## Build

- Install and setup [pico-sdk](https://github.com/raspberrypi/pico-sdk#getting-the-latest-sdk-code)
- Set the `PICO_SDK_PATH` to match, see `pico-sdk` repo above
- Create a build folder, run cmake, then run make

```sh
$ mkdir build
$ cd build
$ cmake ../
$ make
```

In the future, you can skip ahead to the `make` command when you have code changes.

Afterward, within the `build/src` folder, use the `.uf2` file for programming

## Programming The RP2040

- Hold the `BOOTSEL` button on the off RP2040
- Plug the RP2040 in to the computer via USB while holding `BOOTSEL` - Release `BOOTSEL` - After a moment, it will show up as a storage device. - Copy the `.uf2` file to the storage device

The RP2040 will reboot, starting code

## Credits

Thanks to [MrYsLab's NeoPixelConnect](https://github.com/MrYsLab/NeoPixelConnect) library, which has been copied here with minor modifications to make it non-specific to Arduino.
