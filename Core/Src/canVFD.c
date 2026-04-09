#include "canVFD.h"

uint8_t		RxData[8]; // 8 bytes
FDCAN_RxHeaderTypeDef RxHeader;

int head = 0;
int tail = 0;

CAN_Message canQ[CAN_QUEUE_SIZE];

void testFunction(int *test) {
	(*test)++;
}

uint16_t concatenate(uint8_t x, uint8_t y) {
	uint16_t pow = 10;
	while (y >= pow) {
		pow *= 10;
	}
	return x * pow + y;
}

void process_CAN_msgs(void) {
	while (tail != head) {
		CAN_Message currentMessage = canQ[tail];
		tail = (tail + 1) % CAN_QUEUE_SIZE;

		//other can peripherals can be added here as cases w/ their ID
		switch (currentMessage.id) {
		//KELLY VFD - 0x10F8109A (driving direction, speed in rpm)
		case 0x10F8109A:
			speedLSB = currentMessage.data[1];	// LSB of speed 1 RPM/bit
			speedMSB = currentMessage.data[2];  // MSB of speed
			errorCode = currentMessage.data[3];	//see table 1 of Kelly VFD datasheet
			break;
		//KELLY VFD - 0x10F8108D (battery voltage, motor current, motor temp, controller temp)
		case 0x10F8108D:
			batteryVoltageLSB = currentMessage.data[0];	// 0.1 V/bit
			batteryVoltageMSB = currentMessage.data[1];
			motorCurrentLSB = currentMessage.data[2];	// 0.1 A.bit
			motorCurrentMSB = currentMessage.data[3];
			motorTempLSB = currentMessage.data[4]; 		// 0.1 C/bit
			motorTempMSB = currentMessage.data[5];
			controllerTempLSB = currentMessage.data[6]; 	// 0.1 C/bit
			controllerTempMSB = currentMessage.data[7];
			break;
		case 0x10DEADBE:
			lowestCellVoltage = concatenate(currentMessage.data[0], currentMessage.data[1]);
			avgCellVoltage = concatenate(currentMessage.data[2], currentMessage.data[3]);
			highestCellVoltage = concatenate(currentMessage.data[4], currentMessage.data[5]);
			packSOC = currentMessage.data[6];
			bmsTestCounter = currentMessage.data[7];
			break;
		case 0x18FF01F4:
			insulationResistance = concatenate(currentMessage.data[0], currentMessage.data[1]);
			iso_status = currentMessage.data[2];
			imd_counter = currentMessage.data[3];
			imd_warnings = concatenate(currentMessage.data[4], currentMessage.data[5]);
			deviceActivity = currentMessage.data[6];
			break;



		}
	}
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) { // RxFifo0ITs using interrupt 0
	//overriding the builtin function that runs when a CAN interrupt is detected
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE)) { //If Interrupt flag is 1 AND there is a new message in the queue...

		// While loop to pop messages from the queue (useful in event of multiple msg arriving at once
		while (HAL_FDCAN_GetRxFifoFillLevel(hfdcan, FDCAN_RX_FIFO0) > 0) {

			if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
				Error_Handler();
			}

		  	int next = (head + 1) % CAN_QUEUE_SIZE;

		  	if (next != tail) {
		  		canQ[head].id = RxHeader.Identifier;
		  		for (int i = 0; i < 8; i++) {
		  			canQ[head].data[i] = RxData[i];
		  		}
		  		head = next;
		  	}
		}
	}
}
