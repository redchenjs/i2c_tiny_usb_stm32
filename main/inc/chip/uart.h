/*
 * uart.h
 *
 *  Created on: 2020-03-25 12:43
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef INC_CHIP_UART_H_
#define INC_CHIP_UART_H_

#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern void uart1_reset(void);
extern void uart1_init(void);
extern void uart2_init(void);

#endif /* INC_CHIP_UART_H_ */
