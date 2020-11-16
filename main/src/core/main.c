/*
 * main.c
 *
 *  Created on: 2020-11-16 12:57
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "stm32f1xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#include "chip/uart.h"

extern int app_main(void);
static void app_task(void *pvParameter)
{
    (void)pvParameter;

    app_main();

    vTaskDelete(NULL);
}

int __attribute__((used)) _write(int file, char *ptr, int len)
{
    (void)file;

    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);

    return len;
}

void _init(void) {}

int main(void)
{
    /* Configure Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();

    /* Set Interrupt Group Priority */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
    HAL_InitTick(TICK_INT_PRIORITY);

    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

    xTaskCreate(app_task, "appT", 128, NULL, configMAX_PRIORITIES - 1, NULL);

    vTaskStartScheduler();

    return 0;
}
