#ifndef CAN
#define CAN

#include <stdio.h>
#include <stdint.h>

#include "stm32h7xx_hal.h"

#define CAN_QUEUE_SIZE 32
#define UNKNOWN_LOG_SIZE 32

typedef struct {
	uint32_t id;
	uint8_t data[8];
} CAN_Message;

extern CAN_Message canQ250[CAN_QUEUE_SIZE]; //declaring array of can messages
extern volatile int head250;
extern volatile int tail250;

extern CAN_Message canQ500[CAN_QUEUE_SIZE]; //declaring array of can messages
extern volatile int head500;
extern volatile int tail500;
extern int x;

extern uint8_t speedLSB;
extern uint8_t speedMSB;
extern uint16_t totalspeed; //sofia addition
extern uint8_t errorCode;
extern uint8_t batteryVoltageLSB;
extern uint8_t batteryVoltageMSB;
extern uint8_t motorCurrentLSB;
extern uint8_t motorCurrentMSB;
extern uint8_t motorTempLSB;
extern uint8_t motorTempMSB;
extern uint8_t controllerTempLSB;
extern uint8_t controllerTempMSB;

extern uint8_t packSOC;
extern uint16_t lowestCellVoltage;
extern uint16_t avgCellVoltage;
extern uint16_t highestCellVoltage;
extern int bmsTestCounter;

extern uint16_t insulationResistance;
extern uint8_t iso_status;
extern uint8_t imd_counter;
extern uint16_t imd_warnings;
extern uint8_t deviceActivity;

uint16_t concatenate(uint8_t x, uint8_t y);

void process_CAN250_msgs(void);

int isDuplicate(uint32_t id);

void process_CAN500_msgs(void);

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan1, uint32_t RxFifo0ITs);// RxFifo0ITs using interrupt 0

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan2, uint32_t RxFifo1ITs);

#endif
