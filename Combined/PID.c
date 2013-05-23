/*
 * PID.c
 * Author: Yan Gouts
 * Project: Tissue Tester
 *
 * This file includes the definitions of the functions related to the PID controller which are called in the main.c file
 * of the Tissue Tester project. This file uses the struct pid which is defined in PID.h
 *
 */

#include <math.h>
#include "PID.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "newpins.h"
#include "utils/uartstdio.h"
#include "utils/sine.h"
#include "PWMSetup.h"

#define MAX_IVALUE 1024				//Arbitrarily set, needs testing.
#define ACTUALKI 3.0/100000
#define UMAX 1024.0
#define PIDFREQ 8000
#define PI 3.14159

extern long ulCurrent[1];				//Stores data read from ADC0 FIFO
extern long ulLVDT[1];				    //Stores data read from ADC1 FIFO
extern long setPoint;					//Stores the desired set point of the indenter
extern unsigned long pwmPeriod;
long LVDTPIDOutput[1] = {0};			//Output from the LVDT controller
long yLast = 0;
float sumError = 0.0;
PID LVDTController;						//PID struct LVDTController
float Kp = 0.1;
float Ki = ACTUALKI;//3.0/100000; // 7.69/100000
float Kd = 0;
float maxSumError = 0.0;
unsigned long t = 0;

unsigned long roundNum(float num){
	if (num > (long)num){
		return (long)num + 1;
	}else{
		return (long)num;
	}
}

void LVDTPIDInit(void) {
	//Initialize the controller's gains as well as input and output
	LVDTController.Kd = &Kd;
	LVDTController.Ki = &Ki;
	LVDTController.Kp = &Kp;
	LVDTController.x = &setPoint;
	LVDTController.y = ulLVDT;
	LVDTController.yLast = &yLast;
	LVDTController.u = LVDTPIDOutput;
	LVDTController.sumError = &sumError;
	LVDTController.maxSumError = &maxSumError;

	maxSumError = MAX_IVALUE / ((ACTUALKI) + 1.0);

	//Enable the Timer 2 interrupt so that the PID code triggers off
	//of the PWM timer.
	TimerLoadSet(TIMER2_BASE, TIMER_A, pwmPeriod*10);
	IntEnable(INT_TIMER2A);
	IntPrioritySet(INT_TIMER2A, 0x00);
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerEnable(TIMER2_BASE, TIMER_A);
}

interrupt void PIDIntHandlerLVDT(void) {
	float pValue = 0.0;
	float dValue = 0.0;
	float iValue = 560.0;
	float error = 0.0;
	float DCoffset = 0.0;
	float temp = 0.0;

	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	*LVDTController.x = 1096 * (sin((2*PI*t)*10/PIDFREQ) + 1) + 1096;
	t++;
	//Compute the error
	error = *LVDTController.x - *LVDTController.y;

	//Compute the P portion of the controller
	pValue = (*LVDTController.Kp)*error;

	//Compute the D portion of the controller

	/*
	 * Non-complicated D term
	 */
	dValue = (*LVDTController.Kd)*(*LVDTController.yLast - *LVDTController.y);
	*LVDTController.yLast = *LVDTController.y;

	//Compute the I portion of the controller
	temp = (*LVDTController.sumError + error)*Ki;

	if (temp > MAX_IVALUE) {
		iValue = MAX_IVALUE;
		*LVDTController.sumError = MAX_IVALUE / Ki;
	}
	else if (temp < -(MAX_IVALUE)) {
		iValue = -MAX_IVALUE;
		*LVDTController.sumError = -(MAX_IVALUE / Ki);
	}
	else {

		*LVDTController.sumError = *LVDTController.sumError + error;
		iValue = (*LVDTController.Ki)*(*LVDTController.sumError);
	}

	//Calculate controller effort
	(*LVDTController.u) = pValue + dValue + iValue + DCoffset;

	if(*LVDTController.u > (UMAX)){
		pwmSetDuty((UMAX));
	} else if (*LVDTController.u < -(UMAX)){
		pwmSetDuty(-(UMAX));
	}else{
		pwmSetDuty(roundNum(*LVDTController.u));
	}
}
