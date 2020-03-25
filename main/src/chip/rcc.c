/*
 * rcc.c
 *
 *  Created on: 2020-03-24 20:28
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "stm32f1xx_hal.h"

#include "core/os.h"

#define TAG "rcc"

void rcc_init(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();

    RCC_OscInitTypeDef osc_conf = {
        .OscillatorType = RCC_OSCILLATORTYPE_HSE,
        .HSEState = RCC_HSE_ON,
        .HSEPredivValue = RCC_HSE_PREDIV_DIV1,
        .HSIState = RCC_HSI_ON,
        .PLL.PLLState = RCC_PLL_ON,
        .PLL.PLLSource = RCC_PLLSOURCE_HSE,
        .PLL.PLLMUL = RCC_PLL_MUL9,
    };
    HAL_RCC_OscConfig(&osc_conf);

    RCC_ClkInitTypeDef clk_conf = {
        .ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
        .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
        .AHBCLKDivider = RCC_SYSCLK_DIV1,
        .APB1CLKDivider = RCC_HCLK_DIV2,
        .APB2CLKDivider = RCC_HCLK_DIV1,
    };
    HAL_RCC_ClockConfig(&clk_conf, FLASH_LATENCY_2);

    HAL_RCC_EnableCSS();

    SystemCoreClockUpdate();
}
