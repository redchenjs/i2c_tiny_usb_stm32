/*
 * uart.c
 *
 *  Created on: 2020-03-25 12:43
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "stm32f1xx_hal.h"

// PA9  ------> USART1_TX

UART_HandleTypeDef huart1 = {
    .Instance = USART1,
    .Init.BaudRate = 115200,
    .Init.WordLength = UART_WORDLENGTH_8B,
    .Init.StopBits = UART_STOPBITS_1,
    .Init.Parity = UART_PARITY_NONE,
    .Init.Mode = UART_MODE_TX_RX,
    .Init.HwFlowCtl = UART_HWCONTROL_NONE,
    .Init.OverSampling = UART_OVERSAMPLING_16
};

void uart1_init(void)
{
    __HAL_RCC_USART1_CLK_ENABLE();
    HAL_UART_Init(&huart1);

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef io_conf = {
        .Pin = GPIO_PIN_9,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_HIGH
    };
    HAL_GPIO_Init(GPIOA, &io_conf);
}
