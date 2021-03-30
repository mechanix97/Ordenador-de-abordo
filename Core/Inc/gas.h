#ifndef _GAS_H_
#define  _GAS_H_

#include <stdint.h>

#include "stm32f1xx_hal.h"

extern ADC_HandleTypeDef hadc2;

void GAS_init();
uint32_t GAS_read();

#endif
