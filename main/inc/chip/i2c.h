/*
 * i2c.h
 *
 *  Created on: 2020-10-03 13:13
 *      Author: Jack Chen <redchenjs@live.com>
 */

#ifndef INC_CHIP_I2C_H_
#define INC_CHIP_I2C_H_

#include "stm32f1xx_hal.h"

extern I2C_HandleTypeDef i2c1;

extern void i2c1_reset(void);
extern void i2c1_init(void);

#endif /* INC_CHIP_I2C_H_ */
