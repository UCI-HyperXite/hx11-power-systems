#ifndef ENCODER
#define ENCODER


#include <stdio.h>

#include "stm32h7xx_hal.h"


extern TIM_HandleTypeDef htim2; //defined in main

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

#endif
