#ifndef THROTTLE
#define THROTTLE


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "stm32h7xx_hal.h"

extern DAC_HandleTypeDef hdac1; //defined in main

void throttleTest();

#endif
