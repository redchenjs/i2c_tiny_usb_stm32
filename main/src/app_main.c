/*
 * app_main.c
 *
 *  Created on: 2020-03-24 18:33
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "core/os.h"

#include "chip/rcc.h"
#include "chip/i2c.h"
#include "chip/usbd.h"
#include "chip/uart.h"

#include "user/led.h"
#include "user/usb.h"
#include "user/usb_cdc.h"

static void core_init(void)
{
    os_init();
}

static void chip_init(void)
{
    rcc_init();

    usbd_init();

    i2c1_init();

    uart1_init();

    uart2_init();
}

static void board_init(void) {}

static void user_init(void)
{
    led_init();

    usb_init();

    usb_cdc_init();

    os_start();
}

int main(void)
{
    core_init();

    chip_init();

    board_init();

    user_init();

    return 0;
}
