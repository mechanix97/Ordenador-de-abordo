#ifndef _RPM_H_
#define _RPM_H_

#include <stdint.h>

#include "stm32f1xx_hal.h"

extern ADC_HandleTypeDef hadc1;

void RPM_init();
uint32_t RPM_read();

#endif
