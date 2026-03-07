#ifndef VFDCAN
#define VFDCAN

#include <stdio.h>
#include <stdint.h>

#include "stm32h7xx_hal.h"

#define CAN_QUEUE_SIZE 32

typedef struct {
	uint32_t id;
	uint8_t data[8];
} CAN_Message;

extern CAN_Message canQ[CAN_QUEUE_SIZE]; //declaring array of can messages
extern int head;
extern int tail;

void testFunction(int *test);

void process_CAN_msgs(void);

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);// RxFifo0ITs using interrupt 0




#endif
