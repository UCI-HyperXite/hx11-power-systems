#include "throttleDriver.h"

#define MAX_VALUE 4095 //granularity = 12 bits, 4096 increments (1.22mV per change)
#define ENCODER_MAX 100 /*PLACEHOLDER*/

//resources used: https://wiki.st.com/stm32mcu/wiki/Getting_started_with_DAC
void throttleTest() {
		//start throttle at 0V, jump in increments of 1.22mV to 5V.
		for (int dac_value = 0; dac_value <= MAX_VALUE; dac_value++) {
			//sets DAC_CHANNEL_2 to voltage at dac_value (built-in DAC converts
			//channel_2 outputting on PA5
			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value); //do we need 8-bits or 12-bits?
			if (dac_value < 4095) { //255 if using 8-bit
				dac_value++;
			} else {
				dac_value=0;
			}
			HAL_Delay(1);
		}

		//decrement test
		for (int dac_value = MAX_VALUE; dac_value >= 0; dac_value++) {
			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value); //do we need 8-bits or 12-bits?
				if (dac_value > 0) { //255 if using 8-bit
					dac_value--;
				} else {
					dac_value=MAX_VALUE;
				}
				HAL_Delay(1);
		}
}


void throttleDrive(int encoderRawVal) {
	int32_t targetValue = encoderRawVal/ENCODER_MAX * MAX_VALUE;



	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, targetValue);



}








/*
void encoderSimThread(bool encoderRunning) { //why is this a thread?
	if (encoderRunning) {
		  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	} else {
		HAL_TIM_Encoder_Stop(&htim2, TIM_CHANNEL_ALL);
	}
}

void clamp(int32_t *input) {
	if (*input < 0) {
		*input = 0;
	} else if (*input > MAX_VALUE) {
		*input = MAX_VALUE;
	}
}

bool setThrottle(int value) {
	clamp(&value);


	bool success = false;
	//write to the DAC - what pins is the DAC idk
		//need 2 pins for DAC
	//if writing was success: success == 1 else 0

	return success;
}


int main() {
	bool encoderRunning = false;
	//disengage brakes
	printf("Starting throttle and encoder simulation...\n");

	if (!setThrottle(0)) {
		printf("Error: Failed to set throttle to 0\n");
	} else {
		printf("Throttle set to: 0\n");
	}

	//what is pin 17?

	encoderRunning = true;

	//does it need to be in a thread... if yes need to switch to c++
	encoderSimThread(encoderRunning);

	 //don't need to set value in a for loop, we can use the built in encoder position getting
	 //STM32 black magic

//	while (1) {
//		int32_t value = (int32_t)__HAL_TIM_GET_COUNTER(&htim2);
//		if (value < MAX_VALUE && value > 0) {
//			if (!setThrottle(value)) {
//				printf("Error: Failed to set throttle to %d\n", value);
//			} else {
//				printf("Throttle set to: %d\n", value);
//			}
//		}
//	}

	for (int value = 0; value < MAX_VALUE; value += 50) {
		if (!setThrottle(value)) {
			printf("Error: Failed to set throttle to %d\n", value);
		} else {
			printf("Throttle set to: %d\n", value);
		}
		//sleep thread?
	}

	//ramp down
	for (int value = MAX_VALUE*0.5; value >= 0; value -= 100) {
		if (!setThrottle(value)) {
				printf("Error: Failed to set throttle to %d\n", value);
			} else {
				printf("Throttle set to: %d\n", value);
			}
		//sleep thread?
	}

	encoderRunning = false;
	//engage brakes
	printf("DAC Test complete.\n");
	return 0;
}*/s



