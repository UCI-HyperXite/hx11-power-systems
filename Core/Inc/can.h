#ifndef CAN
#define CAN

#include <stdio.h>
#include <stdint.h>

#include "stm32h7xx_hal.h"

#define CAN_QUEUE_SIZE 32
#define UNKNOWN_LOG_SIZE 32
#define RADIUS 0.00395

typedef struct {
	uint32_t id;
	uint8_t data[8];
} CAN_Message;


typedef struct{
	// direction
	uint8_t drivingDirection;
	// in m/s
	double encoderSpeed;
	// error code (units/decoding not done)
	uint8_t errorCode;
	// in V
	double batteryVoltage;
	// in A
	double motorCurrent;
	// in C (to be converted to F)
	double motorTemp;
	// in C (to be converted to F)
	double controllerTemp;
} VFD_CAN_Data;

extern VFD_CAN_Data vfdData;


typedef struct{
	// in V
	double lowestCellVoltage;
	// in V
	double highestTemp;
	// in V
	double highestCellVoltage;
	double packSOC;
	uint8_t bmsTestCounter;
	uint8_t relayStatus;
	double packVoltage;
	double lowestTemp;

}BMS_CAN_Data;

typedef struct{
	double insulationResistance;
	double iso_status;
	double imd_counter;
	double imd_warnings;
	double deviceActivity;
}IMD_CAN_Data;

extern CAN_Message canQ250[CAN_QUEUE_SIZE]; //declaring array of can messages
extern volatile int head250;
extern volatile int tail250;

extern CAN_Message canQ500[CAN_QUEUE_SIZE]; //declaring array of can messages
extern volatile int head500;
extern volatile int tail500;

// debugging variables
// delete later
extern volatile int x;
extern volatile int y;
extern volatile int z;
extern volatile int a;


double concatenate(uint8_t x, uint8_t y);

double rpm_to_ms(double rpm);

void process_CAN250_msgs(VFD_CAN_Data *vfdData);

int isDuplicate(uint32_t id);

void process_CAN500_msgs(BMS_CAN_Data *bmsData, IMD_CAN_Data *imdData);

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan1, uint32_t RxFifo0ITs);// RxFifo0ITs using interrupt 0

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan2, uint32_t RxFifo1ITs);

#endif
