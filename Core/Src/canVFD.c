#include "canVFD.h"

uint8_t		RxData[8]; // 8 bytes
FDCAN_RxHeaderTypeDef RxHeader;

enum drivingDirection {
	FORWARD,
	NEUTRAL,
	REVERSE
};

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) { // RxFifo0ITs using interrupt 0
	//overwriting the builtin function that runs when a CAN interrupt is detected

	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) == 0)
	{
		return;
	}

	// While loop to pop messages from the queue (useful in event of multiple msg arriving at once
	while (HAL_FDCAN_GetRxFifoFillLevel(hfdcan, FDCAN_RX_FIFO0) > 0) {
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
			Error_Handler();
		}

		if (RxHeader.IdType == FDCAN_EXTENDED_ID) {
			switch (RxHeader.Identifier) {
			//other can peripherals can be added here
				//KELLY VFD - 0x10F8109A (driving direction, speed in rpm)
				case 0x10F8109A:
					enum drivingDirection driveDirection = RxData[0]; // per Kelly VFD spec sheet
					uint8_t speedLSB = RxData[1];	// 1 RPM/bit
					uint8_t speedMSB = RxData[2];
					uint8_t errorCode = RxData[3];	//see table 1 of Kelly VFD datasheet

					//IMPORTANT: PROOF OF CONCEPT, should NOT have prints inside ISR
					printf("Driving Direction: %s\n", driveDirection);

					printf("Current Motor Speed: %d", speedMSB);
					printf("%d\n", speedLSB);

					printf("Error Code: %d", errorCode);

				//KELLY VFD - 0x10F8108D (battery voltage, motor current, motor temp, controller temp)
				case 0x10F8108D:

					uint8_t batteryVoltageLSB = RxData[0];	// 0.1 V/bit
					uint8_t batteryVoltageMSB = RxData[1];
					uint8_t motorCurrentLSB = RxData[2];	// 0.1 A.bit
					uint8_t motorCurrentMSB = RxData[3];
					uint8_t motorTempLSB = RxData[4]; 		// 0.1 C/bit
					uint8_t motorTempMSB = RxData[5];
					uint8_t controllerTempLSB = RxData[6]; 	// 0.1 C/bit
					uint8_t controllerTempMSB = RxData[7];

					printf("Battery Voltage: %d", batteryVoltageMSB);
					printf("%d\n", batteryVoltageLSB);

					printf("Motor Current: %d", motorCurrentMSB);
					printf("%d\n", motorCurrentLSB);

					printf("Motor Temperature: %d", motorTempMSB);
					printf("%d\n", motorTempLSB);

					printf("Controller Temperature: %d", controllerTempMSB);
					printf("%d\n", controllerTempLSB);
			}
		}
	}
}
