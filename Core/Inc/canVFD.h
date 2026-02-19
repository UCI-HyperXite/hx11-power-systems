#ifndef VFDCAN
#define VFDCAN

#include <stdio.h>
#include <stdint.h>

#include "main.h"

#define CAN_QUEUE_SIZE 32

typedef struct {
	uint32_t id;
	uint8_t data[8];
} CAN_Message;

extern CAN_Message canQ[CAN_QUEUE_SIZE];
extern int head = 0;
extern int tail = 0;

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);// RxFifo0ITs using interrupt 0

void testFunction(int *test);


#endif
