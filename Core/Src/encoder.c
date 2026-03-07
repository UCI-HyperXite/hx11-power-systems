#include "encoder.h"

int16_t counter = 0;
int16_t count = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	counter = __HAL_TIM_GET_COUNTER(htim);
	count = count/4;
}
