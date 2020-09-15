/*
 * usbd.c
 *
 *  Created on: 2020-03-24 20:36
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "stm32f1xx_hal.h"

// PA11 ------> USB_DM
// PA12 ------> USB_DP

void usbd_init(void)
{
    __HAL_RCC_USB_CLK_ENABLE();

    RCC_PeriphCLKInitTypeDef clk_conf = {
        .PeriphClockSelection = RCC_PERIPHCLK_USB,
        .UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5,
    };
    HAL_RCCEx_PeriphCLKConfig(&clk_conf);

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef io_conf = {
        .Pin = GPIO_PIN_11 | GPIO_PIN_12,
        .Mode = GPIO_MODE_OUTPUT_OD,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_HIGH,
    };
    HAL_GPIO_Init(GPIOA, &io_conf);

    HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 5, 0);
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_SetPriority(USBWakeUp_IRQn, 5, 0);
}
