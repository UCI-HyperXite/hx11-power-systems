/*#include "throttleTest.h"

#define MAX_VALUE 100 //change to actual value when i know it


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






}
*/


