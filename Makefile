include $(PWD)/make/top.mk

PROJECT_NAME = i2c_tiny_usb

TUSB_SRC = components/tinyusb

TUSB_VENDOR = st
TUSB_CHIP_FAMILY = stm32_fsdev

FREERTOS_SRC = $(TUSB_SRC)/lib/FreeRTOS/FreeRTOS/Source
FREERTOS_PORT = ARM_CM3

ST_CMSIS = $(TUSB_SRC)/hw/mcu/st/st_driver/CMSIS/Device/ST/STM32F1xx
ST_HAL_DRIVER = $(TUSB_SRC)/hw/mcu/st/st_driver/STM32F1xx_HAL_Driver

# Components
SRC_C += \
	$(TUSB_SRC)/src/tusb.c \
	$(TUSB_SRC)/src/common/tusb_fifo.c \
	$(TUSB_SRC)/src/device/usbd.c \
	$(TUSB_SRC)/src/device/usbd_control.c \
	$(TUSB_SRC)/src/class/msc/msc_device.c \
	$(TUSB_SRC)/src/class/cdc/cdc_device.c \
	$(TUSB_SRC)/src/class/dfu/dfu_rt_device.c \
	$(TUSB_SRC)/src/class/hid/hid_device.c \
	$(TUSB_SRC)/src/class/midi/midi_device.c \
	$(TUSB_SRC)/src/class/usbtmc/usbtmc_device.c \
	$(TUSB_SRC)/src/class/vendor/vendor_device.c \
	$(TUSB_SRC)/src/class/net/net_device.c \
	$(TUSB_SRC)/src/portable/$(TUSB_VENDOR)/$(TUSB_CHIP_FAMILY)/dcd_$(TUSB_CHIP_FAMILY).c \
	$(FREERTOS_SRC)/list.c \
	$(FREERTOS_SRC)/queue.c \
	$(FREERTOS_SRC)/tasks.c \
	$(FREERTOS_SRC)/timers.c \
	$(FREERTOS_SRC)/event_groups.c \
	$(FREERTOS_SRC)/portable/MemMang/heap_4.c \
	$(FREERTOS_SRC)/portable/GCC/$(FREERTOS_PORT)/port.c \
	$(ST_CMSIS)/Source/Templates/system_stm32f1xx.c \
	$(ST_HAL_DRIVER)/Src/stm32f1xx_hal.c \
	$(ST_HAL_DRIVER)/Src/stm32f1xx_hal_cortex.c \
	$(ST_HAL_DRIVER)/Src/stm32f1xx_hal_rcc.c \
	$(ST_HAL_DRIVER)/Src/stm32f1xx_hal_rcc_ex.c \
	$(ST_HAL_DRIVER)/Src/stm32f1xx_hal_gpio.c \
	$(ST_HAL_DRIVER)/Src/stm32f1xx_hal_uart.c \
	$(ST_HAL_DRIVER)/Src/stm32f1xx_hal_i2c.c

SRC_S += \
	$(ST_CMSIS)/Source/Templates/gcc/startup_stm32f103xb.s

LD_FILE = \
	$(ST_CMSIS)/Source/Templates/gcc/linker/STM32F103XB_FLASH.ld

INC += \
	$(TUSB_SRC)/src \
	$(TUSB_SRC)/hw \
	$(TUSB_SRC)/hw/mcu/st/st_driver/CMSIS/Include \
	$(FREERTOS_SRC)/include \
	$(FREERTOS_SRC)/portable/GCC/$(FREERTOS_PORT) \
	$(ST_CMSIS)/Include \
	$(ST_HAL_DRIVER)/Inc

# Main
SRC_C += \
	$(wildcard main/src/*.c) \
	$(wildcard main/src/*/*.c)

INC += main/inc main/inc/core

CFLAGS += \
	-flto \
	-mthumb \
	-mabi=aapcs \
	-mcpu=cortex-m3 \
	-mfloat-abi=soft \
	-nostdlib -nostartfiles \
	-Wno-unused-variable \
	-DSTM32F103xB \
	-DCFG_TUSB_MCU=OPT_MCU_STM32F1 \
	$(addprefix -I,$(INC))

ASFLAGS += $(CFLAGS)

LDFLAGS += \
	$(CFLAGS) \
	-fshort-enums \
	-Wl,-T,$(LD_FILE) \
	-Wl,-Map=$@.map \
	-Wl,-cref \
	-Wl,-gc-sections \
	-Wl,--undefined=vTaskSwitchContext \
	-specs=nosys.specs \
	-specs=nano.specs

LIBS += -lgcc -lc -lm -lnosys

JLINK_DEVICE = stm32f103cb
JLINK_IF = swd

include $(PWD)/make/common.mk
include $(PWD)/make/project.mk
