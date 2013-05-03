/*
 * PID.c
 * Author: Yan Gouts
 * Project: Tissue Tester
 *
 * This file includes the definitions of the functions related to the PID controller which are called in the main.c file
 * of the Tissue Tester project. This file uses the struct pid which is defined in PID.h
 *
 */

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
#include "PWMSetup.h"

#define MAX_ARRAY_SIZE 20

extern long ulCurrent[1];				//Stores data read from ADC0 FIFO
extern long ulLVDT[1];				    //Stores data read from ADC1 FIFO
extern long setPoint;					//Stores the desired set point of the indenter
extern float Kp;
extern float Ki;
extern float Kd;
extern unsigned long pwmPeriod;
long LVDTPIDOutput[1] = {0};			//Output from the LVDT controller
long yLast = 0;
PID LVDTController;						//Pointer to the PID struct LVDTController
int arrayIndex = 0;						//Index for the integral array

void LVDTPIDInit(void) {
	//Initialize the controller's gains as well as input and output
	LVDTController.Kd = &Kd;
	LVDTController.Ki = &Ki;
	LVDTController.Kp = &Kp;
	LVDTController.x = &setPoint;
	LVDTController.y = ulLVDT;
	LVDTController.yLast = &yLast;
	LVDTController.u = LVDTPIDOutput;

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
	float error = 0.0;
	float DCoffset = 0;
	//float Td = (*LVDTController.Kd)/(*LVDTController.Kp);
	//int N = 15;
	//float samplingTime = 1/7000;

	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

	//Compute the error
	error = *LVDTController.y - *LVDTController.x;

	//Compute the P portion of the controller
	pValue = (*LVDTController.Kp)*error;

	//Compute the D portion of the controller
	/*
	 * Complicated D term
	 */
	//dValue[1] = (Td/(Td+N*samplingTime))*dValue[0] -

	/*
	 * Non-complicated D term
	 */
	dValue = (*LVDTController.Kd)*(*LVDTController.yLast - *LVDTController.y);
	*LVDTController.yLast = *LVDTController.y;

	(*LVDTController.u) = pValue + dValue;
	pwmSetDuty(*LVDTController.u);
	//pwmSetDuty(setPoint);
}
