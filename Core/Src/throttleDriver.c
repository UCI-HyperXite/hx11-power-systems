#include "throttleDriver.h"

#define MAX_VALUE 4095 //granularity = 12 bits, 4096 increments (1.22mV per change)
#define ENCODER_MAX 100 /*PLACEHOLDER*/

//resources used: https://wiki.st.com/stm32mcu/wiki/Getting_started_with_DAC
void throttleTest() {
		//start throttle at 0V, jump in increments of 1.22mV to 5V.
		for (int dac_value = 0; dac_value <= MAX_VALUE; dac_value++) {
			//sets DAC_CHANNEL_2 to voltage at dac_value (built-in DAC converts
			//channel_2 outputting on PA5
			//HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value); //do we need 8-bits or 12-bits?
			if (dac_value < 4095) { //255 if using 8-bit
				dac_value++;
			} else {
				dac_value=0;
			}
			HAL_Delay(1);
		}

		//decrement test
		for (int dac_value = MAX_VALUE; dac_value >= 0; dac_value++) {
			//HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value); //do we need 8-bits or 12-bits?
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



	//HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, targetValue);



}
