/*
 * usb_cdc.c
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

static HAL_StatusTypeDef status = HAL_OK;
static uint8_t buff[CFG_TUD_ENDPOINT0_SIZE] = {0};

static uint8_t uart_configured = 0;

// Invoked when received new data
void tud_cdc_rx_cb(uint8_t itf)
{
    (void)itf;
    uint32_t size = tud_cdc_available();

    tud_cdc_read(buff, size);

    if (uart_configured) {
        HAL_UART_Transmit(&huart1, (uint8_t *)buff, size, 500);
    }
}

// Invoked when line coding is change via SET_LINE_CODING
void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const *p_line_coding)
{
    (void)itf;

    huart1.Init.BaudRate = p_line_coding->bit_rate;

    switch (p_line_coding->stop_bits) {
    default:
    case 0: // 1 stop bit
        huart1.Init.StopBits = UART_STOPBITS_1;
        break;
    case 1: // 1.5 stop bits
    case 2: // 2 stop bits
        huart1.Init.StopBits = UART_STOPBITS_2;
        break;
    }

    switch (p_line_coding->parity) {
    default:
    case 0: // None
        huart1.Init.Parity = UART_PARITY_NONE;
        break;
    case 1: // Odd
        huart1.Init.Parity = UART_PARITY_ODD;
        break;
    case 2: // Even
        huart1.Init.Parity = UART_PARITY_EVEN;
        break;
    }

    uart1_reset();

    uart_configured = 1;
}

void usb_cdc_task(void *pvParameter)
{
    (void)pvParameter;

    while (1) {
        if (tud_cdc_connected() && uart_configured) {
            char byte = 0;

            status = HAL_UART_Receive(&huart1, (uint8_t *)&byte, 1, 0);

            if (status == HAL_OK) {
                tud_cdc_write_char(byte);
                tud_cdc_write_flush();
            }
        } else {
            uart_configured = 0;
        }

        taskYIELD();
    }
}

void usb_cdc_init(void)
{
    xTaskCreate(usb_cdc_task, "usbCdcT", 128, NULL, configMAX_PRIORITIES - 4, NULL);
}
