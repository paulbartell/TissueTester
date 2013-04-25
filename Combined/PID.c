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

#define MAX_ARRAY_SIZE 20

extern long ulCurrent[1];				//Stores data read from ADC0 FIFO
extern long ulLVDT[1];				    //Stores data read from ADC1 FIFO
extern long setPoint;					//Stores the desired set point of the indenter
extern float Kp;
extern float Ki;
extern float Kd;
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

}

void PIDIntHandlerLVDT(void) {
	float pValue = 0.0;
	float dValue = 0.0;
	float error = 0.0;
	//float Td = (*LVDTController.Kd)/(*LVDTController.Kp);
	//int N = 15;
	//float samplingTime = 1/7000;

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

}
