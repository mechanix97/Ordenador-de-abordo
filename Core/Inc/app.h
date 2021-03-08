#ifndef _APP_H_
#define _APP_H_

#include <stdio.h>

#include "fonts.h"
#include "ssd1306.h"
#include "sh1106.h"
#include "test.h"

void setup();
void loop();

void turnOnLedPWM1(uint16_t leds);
void turnOnLedPWM2(uint16_t leds);

void debugPrint(UART_HandleTypeDef *huart, char _out[]);
void debugPrintln(UART_HandleTypeDef *huart, char _out[]);

#endif
