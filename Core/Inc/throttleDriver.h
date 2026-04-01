#ifndef THROTTLE
#define THROTTLE


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "stm32h7xx_hal.h"

#define DAC_MAX_VALUE 4095 //granularity = 12 bits, 4096 increments (1.22mV per change)

#define Kp 0.1 //calibrate based on trial and error from vfd
#define Kd 0.1
#define Ki 0.1
#define dT 0.1 //ALL PLACEHOLDER CHANGE WHEN CALIBRATING VFD


typedef struct {
    float target;
    float kp, ki, kd;
    float integral;
    float prev_error;
    float dt;
    float output_min;
    float output_max;
} PID;

extern DAC_HandleTypeDef hdac1; //defined in main

void throttleTest();

#endif
