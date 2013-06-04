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
#include "utils/uartstdio.h"
//#include "utils/sine.h"
#include "PWM.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "buttons.h"

#define MAX_IVALUE 1024				//Arbitrarily set, needs testing.
#define ACTUALKI 3.0/100000
#define UMAX 1024.0
#define PIDFREQ 8000
#define PI 3.14159

extern long ulCurrent[1];				//Stores data read from ADC0 FIFO
extern long ulLVDT[1];				    //Stores data read from ADC1 FIFO
signed long setPoint;					//Stores the desired set point of the indenter
extern unsigned long pwmPeriod;
signed long LVDTPIDOutput[1] = {0};			//Output from the LVDT controller
PID LVDTController;						//PID struct LVDTController
PID IController;
PID OLController;
PID *Controller = &IController; 		// Set default controller type
unsigned long t = 0;
unsigned long freq = 1;
extern long ButtonStatus;


unsigned long roundNum(float num){
	if (num > (long)num){
		return (long)num + 1;
	}else{
		return (long)num;
	}
}

void PIDEnable(void);


void LVDTPIDInit(void) {
	//Initialize the controller's gains as well as input and output
	LVDTController.Kd = .000000000000001;
	LVDTController.Ki = 0.0000000000003;
	LVDTController.Kp = 0.3;
	LVDTController.x = &setPoint;
	LVDTController.y = &ulLVDT;
	LVDTController.yLast = 0;
	LVDTController.u = 0;
	LVDTController.sumError = 0.0;
	LVDTController.maxSumError = MAX_IVALUE / ((ACTUALKI) + 1.0);
}


void IPIDInit(void) {
	//Initialize the controller's gains as well as input and output
	IController.Kd = 1.0/100000000;
	IController.Ki = 1.0/1000000;
	IController.Kp = 0.45;
	IController.x = &setPoint;
	IController.y = &ulCurrent;
	IController.yLast = 0;
	IController.u = 0;
	IController.sumError = 0.0;
	IController.maxSumError = MAX_IVALUE / ((ACTUALKI) + 1.0);
}

void OLInit(void) {
	//Initialize the controller's gains as well as input and output
	OLController.Kd = 0.0;
	OLController.Ki = 0.0;
	OLController.Kp = 0.0;
	OLController.x = &setPoint;
	OLController.y = ulCurrent;
	OLController.yLast = 0;
	OLController.u = 0;
	OLController.sumError = 0.0;
	OLController.maxSumError = MAX_IVALUE / ((ACTUALKI) + 1.0);
	OLController.offset = 0.0;
}

void setController(unsigned long cont){
	if(cont == 0x00){
		Controller = &LVDTController;
	} else if(cont == 0x01){
		Controller = &IController;
	} else if(cont == 0x00){
		Controller = &OLController;
	}
}
void PIDInit(void){
	//Enable the Timer 2 interrupt so that the PID code triggers off
	//of the PWM timer.
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerLoadSet(TIMER2_BASE, TIMER_A, pwmPeriod*10);
	IntEnable(INT_TIMER2A);
	IntPrioritySet(INT_TIMER2A, 0x00);
	TimerEnable(TIMER2_BASE, TIMER_A);
	IntMasterEnable();
}



void PIDEnable(void){
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
}

void PIDDisable(void){
	TimerIntDisable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
}

void PIDIntHandler(void) {
	float pValue = 0.0;
	float dValue = 0.0;
	float iValue = 0.0;
	long error = 0.0;
	float DCoffset = 0.0;
	float temp = 0.0;

	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	//*(Controller->x) = (long) (1096 * (sinf((2*PI*t*freq)/PIDFREQ) + 1) + 1096); // Calculate our desired setpoint
	//t++;
	//Compute the error
	error = *(Controller->x) - *(Controller->y);

	//Compute the P portion of the controller
	pValue = (Controller->Kp)*error;

	//Compute the D portion of the controller
	dValue = (Controller->Kd)*(Controller->yLast - *(Controller->y));

	Controller->yLast = *(Controller->y);

	//Compute the I portion of the controller
	temp = (Controller->sumError + error);//*Controller->Ki;

	if (temp > Controller->maxSumError) {
		iValue = Controller->maxSumError*Controller->Ki;
		Controller->sumError = Controller->maxSumError;
	}
	else if (temp < -(MAX_IVALUE)) {
		iValue = -Controller->maxSumError*Controller->Ki;
		Controller->sumError = -(Controller->maxSumError);
	}
	else {
		Controller->sumError = temp;
		iValue = (Controller->Ki)*(Controller->sumError);
	}

	//Calculate controller effort
	(Controller->u) = pValue + dValue + iValue + DCoffset;

	if(Controller->u > (UMAX)){
		pwmSetDuty((UMAX));
	} else if (Controller->u < -(UMAX)){
		pwmSetDuty(-(UMAX));
	}else{
		pwmSetDuty(roundNum(Controller->u));
	}

}
void eStopHandler(void) {
	GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
	ButtonStatus = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
}