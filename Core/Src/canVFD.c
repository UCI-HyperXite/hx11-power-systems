uint8_t		RxData[8]; // 8 bytes
CAN_RxHeaderTypeDef RxHeader;

enum drivingDirection {
	FORWARD,
	NEUTRAL,
	REVERSE
};

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	//overwriting the builtin function that runs when a CAN interrupt is detected
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
		Error_Handler();
	}

	//KELLY VFD - 0x10F8109A (driving direction, speed in rpm)
	if ((RxHeader.ExtID = 0x10F8109A)) { //IDK if RxHeader actually saves this ID here

		enum drivingDirection driveDirection = RxData[0]; // per Kelly VFD spec sheet
		uint8_t speedLSB = RxData[1];	// 1 RPM/bit
		uint8_t speedMSB = RxData[2];
		uint8_t errorCode = RxData[3];	//see table 1 of Kelly VFD datasheet

		printf("Driving Direction: %s\n", driveDirection);

		printf("Current Motor Speed: %d", speedMSB);
		printf("%d\n", speedLSB);

		printf("Error Code: %d", errorCode);

	//KELLY VFD - 0x10F8108D (battery voltage, motor current, motor temp, controller temp)
	} else if (RxHeader.ExtID = 0x10F8108D) {

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
