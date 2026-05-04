/*
 * dac.c
 *
 *  Created on: May 1, 2026
 *      Author: sofia
 */


#include "dac.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>

#define DAC_ADDR (0x60 << 1)
#define DAC_MAX  4095

extern I2C_HandleTypeDef hi2c1;

static void DAC_Write(uint16_t val)
{
    uint8_t data[2];

    if (val > DAC_MAX) val = DAC_MAX;

    data[0] = (val >> 8) & 0x0F;
    data[1] = val & 0xFF;

    HAL_I2C_Master_Transmit(&hi2c1, DAC_ADDR, data, 2, HAL_MAX_DELAY);
}

void DAC_SetHigh(void)
{
    DAC_Write(DAC_MAX);
}

void DAC_SetLow(void)
{
    DAC_Write(0);
}
