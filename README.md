Generic USB-I2C Adapter
=======================

Generic USB-I2C Adapter based on STM32F103CB chip.

## Dependencies

```
JLinkExe
arm-none-eabi-gcc
```

## Preparing

### Obtain the source

```
git clone --recursive https://github.com/redchenjs/i2c_tiny_usb_stm32.git
```

### Update an existing repository

```
git pull
git submodule update --init --recursive
```

## Building

```
make flash
```
