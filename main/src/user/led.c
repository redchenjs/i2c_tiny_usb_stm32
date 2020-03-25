/*
 * led.c
 *
 *  Created on: 2018-02-13 15:43
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "stm32f1xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#include "core/os.h"

#define TAG "led"

static const TickType_t led_mode_table[][2] = {
/*  { active, inactive }  */
    {   2000,     2000 },   // 0
    {   1000,     1000 },   // 1
    {    500,      500 },   // 2
    {    250,      250 },   // 3
    {    100,      100 },   // 4
    {     25,       25 },   // 5
    {    625,       25 },   // 6
    {   1250,       25 },   // 7
    {   1875,       25 },   // 8
    {   2500,       25 },   // 9
};

static uint8_t led_mode_index = 3;

static void led_task(void *pvParameter)
{
    (void)pvParameter;
    uint8_t active = 0;
    portTickType xLastWakeTime;

    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef io_conf = {
        .Pin = GPIO_PIN_13,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_HIGH,
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

        vTaskDelayUntil(&xLastWakeTime, led_mode_table[led_mode_index][active] / portTICK_RATE_MS);
    }
}

void led_set_mode(uint8_t idx)
{
    if (idx >= sizeof(led_mode_table)/2) {
        OS_LOGE(TAG, "invalid mode index");
        return;
    }
    led_mode_index = idx;
}

void led_init(void)
{
    xTaskCreate(led_task, "ledT", 128, NULL, configMAX_PRIORITIES - 2, NULL);
}
