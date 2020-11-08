/*
 * i2c.c
 *
 *  Created on: 2020-10-03 13:13
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "stm32f1xx_hal.h"

// PA6 ------> I2C1_SCL
// PA7 ------> I2C1_SDA

I2C_HandleTypeDef i2c1 = {
    .Instance = I2C1,
    .Init.ClockSpeed = 400000,
    .Init.DutyCycle = I2C_DUTYCYCLE_2,
    .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
    .Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
    .Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,
    .Init.NoStretchMode = I2C_NOSTRETCH_DISABLE,
    .Init.OwnAddress1 = 0x00,
    .Init.OwnAddress2 = 0x00
};

static GPIO_InitTypeDef io_conf = {
    .Pin = GPIO_PIN_6 | GPIO_PIN_7,
    .Mode = GPIO_MODE_AF_OD,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_HIGH
};

void i2c1_reset(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

    io_conf.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(GPIOB, &io_conf);

    CLEAR_BIT(i2c1.Instance->CR1, I2C_CR1_PE);
    asm("nop");

    HAL_I2C_DeInit(&i2c1);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_Delay(1);

    SET_BIT(i2c1.Instance->CR1, I2C_CR1_SWRST);
    asm("nop");

    CLEAR_BIT(i2c1.Instance->CR1, I2C_CR1_SWRST);
    asm("nop");

    SET_BIT(i2c1.Instance->CR1, I2C_CR1_PE);
    asm("nop");

    HAL_I2C_Init(&i2c1);

    io_conf.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOB, &io_conf);
}

void i2c1_init(void)
{
    __HAL_RCC_I2C1_CLK_ENABLE();
    HAL_I2C_Init(&i2c1);

    __HAL_RCC_GPIOB_CLK_ENABLE();
    HAL_GPIO_Init(GPIOB, &io_conf);
}
