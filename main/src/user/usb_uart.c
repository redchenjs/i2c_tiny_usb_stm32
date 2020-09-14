/*
 * usb_uart.c
 *
 *  Created on: 2020-03-24 20:57
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "stm32f1xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#include "tusb.h"

#include "core/os.h"
#include "chip/uart.h"

void tud_cdc_rx_cb(uint8_t itf)
{
    (void)itf;
    uint8_t buff[CFG_TUD_ENDPOINT0_SIZE] = {0};
    uint32_t size = tud_cdc_available();

    tud_cdc_read(buff, size);

    HAL_UART_Transmit(&huart1, (uint8_t *)buff, size, 500);
}

void usb_uart_task(void *pvParameter)
{
    (void)pvParameter;

    while (1) {
        if (tud_cdc_connected()) {
            HAL_StatusTypeDef status = HAL_OK;

            char byte = 0;
            status = HAL_UART_Receive(&huart1, (uint8_t *)&byte, 1, 0);

            if (status == HAL_OK) {
                tud_cdc_write_char(byte);
                tud_cdc_write_flush();
            }
        }

        taskYIELD();
    }
}

void usb_uart_init(void)
{
    xTaskCreate(usb_uart_task, "usbUartT", 128, NULL, configMAX_PRIORITIES - 4, NULL);
}
