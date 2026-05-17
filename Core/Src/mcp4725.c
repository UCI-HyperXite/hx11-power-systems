#include "mcp4725.h"

extern I2C_HandleTypeDef hi2c1;

#define MCP4725_ADDR_0 (0x60 << 1)
#define MCP4725_ADDR_1 (0x61 << 1)

HAL_StatusTypeDef MCP4725_CheckAlive(void)
{
    if (HAL_I2C_IsDeviceReady(&hi2c1, MCP4725_ADDR_0, 3, 10) == HAL_OK)
        return HAL_OK;

    if (HAL_I2C_IsDeviceReady(&hi2c1, MCP4725_ADDR_1, 3, 10) == HAL_OK)
        return HAL_OK;

    return HAL_ERROR;
}
