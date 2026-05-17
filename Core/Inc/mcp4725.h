#ifndef MCP4725_H
#define MCP4725_H

#include "stm32h7xx_hal.h"

HAL_StatusTypeDef MCP4725_CheckAlive(void);
HAL_StatusTypeDef MCP4725_Read(uint16_t *dac, uint16_t *eeprom);

#endif
