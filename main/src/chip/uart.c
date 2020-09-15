/*
 * uart.c
 *
 *  Created on: 2020-03-25 12:43
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "stm32f1xx_hal.h"

// PA9  ------> USART1_TX
// PA10 ------> USART1_RX
// PA2  ------> USART2_TX (for console output)

UART_HandleTypeDef huart1 = {
    .Instance = USART1,
    .Init.BaudRate = 115200,
    .Init.WordLength = UART_WORDLENGTH_8B,
    .Init.StopBits = UART_STOPBITS_1,
    .Init.Parity = UART_PARITY_NONE,
    .Init.Mode = UART_MODE_TX_RX,
    .Init.HwFlowCtl = UART_HWCONTROL_NONE,
    .Init.OverSampling = UART_OVERSAMPLING_16,
};

UART_HandleTypeDef huart2 = {
    .Instance = USART2,
    .Init.BaudRate = 115200,
    .Init.WordLength = UART_WORDLENGTH_8B,
    .Init.StopBits = UART_STOPBITS_1,
    .Init.Parity = UART_PARITY_NONE,
    .Init.Mode = UART_MODE_TX_RX,
    .Init.HwFlowCtl = UART_HWCONTROL_NONE,
    .Init.OverSampling = UART_OVERSAMPLING_16,
};

static DMA_HandleTypeDef hdma_rx = {
    .Instance = DMA1_Channel5,
    .Init.Direction = DMA_PERIPH_TO_MEMORY,
    .Init.PeriphInc = DMA_PINC_DISABLE,
    .Init.MemInc = DMA_MINC_ENABLE,
    .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
    .Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
    .Init.Mode = DMA_NORMAL,
    .Init.Priority = DMA_PRIORITY_HIGH,
};

static GPIO_InitTypeDef io_conf = {
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
};

void uart1_reset(void)
{
    HAL_NVIC_DisableIRQ(USART1_IRQn);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

    io_conf.Pin = GPIO_PIN_9;
    io_conf.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOA, &io_conf);

    HAL_UART_Init(&huart1);
    HAL_Delay(100);

    io_conf.Mode = GPIO_MODE_AF_PP;
    HAL_GPIO_Init(GPIOA, &io_conf);

    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void uart1_init(void)
{
    __HAL_RCC_USART1_CLK_ENABLE();
    HAL_UART_Init(&huart1);

    __HAL_RCC_GPIOA_CLK_ENABLE();

    io_conf.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOA, &io_conf);

    __HAL_RCC_DMA1_CLK_ENABLE();

    HAL_DMA_Init(&hdma_rx);

    __HAL_LINKDMA(&huart1, hdmarx, hdma_rx);

    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void uart2_init(void)
{
    __HAL_RCC_USART2_CLK_ENABLE();
    HAL_UART_Init(&huart2);

    __HAL_RCC_GPIOA_CLK_ENABLE();

    io_conf.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOA, &io_conf);
}
