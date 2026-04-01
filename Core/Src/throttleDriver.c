#include "throttleDriver.h"

uint16_t concatenate(uint8_t x, uint8_t y) {
	uint16_t pow = 10;
	while (y >= pow) {
		pow *= 10;
	}
	return x * pow + y;
}

//resources used: https://wiki.st.com/stm32mcu/wiki/Getting_started_with_DAC
void throttleTest() {
		//start throttle at 0V, jump in increments of 1.22mV to 5V.
		for (int dac_value = 0; dac_value <= DAC_MAX_VALUE; dac_value++) {
			//sets DAC_CHANNEL_2 to voltage at dac_value (built-in DAC converts
			//channel_2 outputting on PA5
			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value);
			if (dac_value < 4095) { //255 if using 8-bit
				dac_value++;
			} else {
				dac_value=0;
			}
			HAL_Delay(1);
		}

		//decrement test
		for (int dac_value = DAC_MAX_VALUE; dac_value >= 0; dac_value++) {
			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value); //do we need 8-bits or 12-bits?
			if (dac_value > 0) { //255 if using 8-bit
				dac_value--;
			} else {
				dac_value= DAC_MAX_VALUE;
			}
			HAL_Delay(1);
		}
}

void PIDInit(PID* pid, int targetRPM) {
    pid->target     = targetRPM;
    pid->kp         = Kp;
    pid->ki         = Ki;
    pid->kd         = Kd;
    pid->integral   = 0.0f;
    pid->prev_error = 0.0f;
    pid->dt         = dT;
    pid->output_min = 0.0f;
    pid->output_max = 1.0f;   //0-1 bounds required for volts to dac conversion
}

float throttlePID(PID* pid, int8_t vfdRawLSB, int8_t vfdRawMSB, int targetRPM) {
	PIDInit(pid, targetRPM);
	int16_t actualRPM = concatenate(vfdRawMSB, vfdRawLSB);
	float P, I, D;

	int error = targetRPM-actualRPM;

	//P-TERM
	P = pid->kp * error;

	/*I-TERM - might not need tbh
	pid->integral += (error * pid->dt);
	//might need to clamp integral term to something
	I = pid->ki * pid->integral;*/

	//D-TERM
	D = pid->kd * (error - pid->prev_error)/(pid->dt);
	pid->prev_error = error;

	float correction = P/* + I*/ + D;

	correction = (correction < pid->output_min && correction > pid->output_max) ? 1 : correction; //if it is out of range the output will be 1
	return correction; //must be a float in range 0.0 - 1.0
}

void driveDAC(float throttle) {
	int dacInput = (int)(throttle * DAC_MAX_VALUE);
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dacInput);
}
