#include "canVFD.h"

uint8_t		RxData[8]; // 8 bytes
FDCAN_RxHeaderTypeDef RxHeader;


void testFunction(int *test) {
	(*test)++;
}

void process_CAN_msgs(void) {
	while (tail != head) {
		CAN_Message currentMessage = canQ[tail];
		tail = (tail + 1) % CAN_QUEUE_SIZE;

		//other can peripherals can be added here as cases w/ their ID
		switch (currentMessage.id) {
		//KELLY VFD - 0x10F8109A (driving direction, speed in rpm)
		case 0x10F8109A:
			//ummmmm controls might want to write this part now... idk how to log dataa
			uint8_t speedLSB = currentMessage.data[1];	// 1 RPM/bit
			uint8_t speedMSB = currentMessage.data[2];
			uint8_t errorCode = currentMessage.data[3];	//see table 1 of Kelly VFD datasheet
		//KELLY VFD - 0x10F8108D (battery voltage, motor current, motor temp, controller temp)
		case 0x10F8108D:
			uint8_t batteryVoltageLSB = currentMessage.data[0];	// 0.1 V/bit
			uint8_t batteryVoltageMSB = currentMessage.data[1];
			uint8_t motorCurrentLSB = currentMessage.data[2];	// 0.1 A.bit
			uint8_t motorCurrentMSB = currentMessage.data[3];
			uint8_t motorTempLSB = currentMessage.data[4]; 		// 0.1 C/bit
			uint8_t motorTempMSB = currentMessage.data[5];
			uint8_t controllerTempLSB = currentMessage.data[6]; 	// 0.1 C/bit
			uint8_t controllerTempMSB = currentMessage.data[7];
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

		  	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1); //LED for physical sign that interrupt is triggered

		  	int next = (head + 1) % CAN_QUEUE_SIZE;

		  	if (next != tail) {
		  		canQ[head].id = RxHeader.Identifier;
		  		memcpy(canQ[head].data, RxData, 8);
		  		head = next;
		  	}
		}
	}
}
