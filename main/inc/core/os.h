/*
 * os.h
 *
 *  Created on: 2020-03-24 21:00
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef INC_CORE_OS_H_
#define INC_CORE_OS_H_

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LOG_RESET_COLOR   "\033[0m"
#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D
#define LOG_COLOR_V

#define LOG_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " (%lu) %s: " format LOG_RESET_COLOR "\r\n"

#define OS_LOGE(tag, format, ...) printf(LOG_FORMAT(E, format), xTaskGetTickCount(), tag, ##__VA_ARGS__)
#define OS_LOGW(tag, format, ...) printf(LOG_FORMAT(W, format), xTaskGetTickCount(), tag, ##__VA_ARGS__)
#define OS_LOGI(tag, format, ...) printf(LOG_FORMAT(I, format), xTaskGetTickCount(), tag, ##__VA_ARGS__)

extern void os_start(void);
extern void os_init(void);

#endif /* INC_CORE_OS_H_ */
