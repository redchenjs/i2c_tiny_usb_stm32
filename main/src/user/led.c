/*
 * led.c
 *
 *  Created on: 2018-02-13 15:43
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include <stdbool.h>

#include "stm32f1xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#include "core/os.h"
#include "user/led.h"

#define TAG "led"

static const TickType_t led_mode_table[][2] = {
    [LED_MODE_IDX_BLINK_S1] = {2000, 2000},
    [LED_MODE_IDX_BLINK_S0] = {1000, 1000},
    [LED_MODE_IDX_BLINK_M1] = { 500,  500},
    [LED_MODE_IDX_BLINK_M0] = { 250,  250},
    [LED_MODE_IDX_BLINK_F1] = { 100,  100},
    [LED_MODE_IDX_BLINK_F0] = {  50,   50},
    [LED_MODE_IDX_PULSE_D0] = { 625,   25},
    [LED_MODE_IDX_PULSE_D1] = {1250,   25},
    [LED_MODE_IDX_PULSE_D2] = {1875,   25},
    [LED_MODE_IDX_PULSE_D3] = {2500,   25}
};

static led_mode_t led_mode = LED_MODE_IDX_BLINK_F0;

static void led_task(void *pvParameter)
{
    (void)pvParameter;
    bool active = false;
    portTickType xLastWakeTime;

    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef io_conf = {
        .Pin = GPIO_PIN_13,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_HIGH
    };
    HAL_GPIO_Init(GPIOC, &io_conf);

    OS_LOGI(TAG, "started.");

    while (1) {
        xLastWakeTime = xTaskGetTickCount();

        if (active) {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        }

        active = !active;

        vTaskDelayUntil(&xLastWakeTime, led_mode_table[led_mode][active] / portTICK_RATE_MS);
    }
}

void led_set_mode(led_mode_t idx)
{
    if (idx >= sizeof(led_mode_table) / sizeof(led_mode_table[0])) {
        OS_LOGE(TAG, "invalid mode index");
        return;
    }

    led_mode = idx;
}

led_mode_t led_get_mode(void)
{
    return led_mode;
}

void led_init(void)
{
    xTaskCreate(led_task, "ledT", 128, NULL, configMAX_PRIORITIES - 2, NULL);
}
