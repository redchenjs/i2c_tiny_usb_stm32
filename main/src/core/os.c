/*
 * os.c
 *
 *  Created on: 2020-03-24 21:00
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "stm32f1xx_hal.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "core/os.h"
#include "chip/uart.h"

#define TAG "os"

const volatile uint8_t freeRTOSMemoryScheme = configUSE_HEAP_SCHEME;

int __attribute__((used)) _write(int file, char *ptr, int len)
{
    (void)file;

    HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, 500);

    return len;
}

void _init(void) {}

void PendSV_Handler(void)
{
    xPortPendSVHandler();
}

void SysTick_Handler(void)
{
    HAL_IncTick();

    xPortSysTickHandler();
}

void SVC_Handler(void)
{
    vPortSVCHandler();
}

void os_start(void)
{
    OS_LOGI(TAG, "FreeRTOS memory scheme: %d", freeRTOSMemoryScheme);
    OS_LOGI(TAG, "Starting scheduler...");

    vTaskStartScheduler();
}

void os_init(void)
{
    /* Configure Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();

    /* Set Interrupt Group Priority */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
    HAL_InitTick(TICK_INT_PRIORITY);

    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
}
