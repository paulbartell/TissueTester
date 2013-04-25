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
long error;
PID LVDTController;
int arrayIndex = 0;						//Index for the error array


void LVDTPIDInit(void) {
	//Initialize the controller's gains as well as input and output
	LVDTController.Kd = &Kd;
	LVDTController.Ki = &Ki;
	LVDTController.Kp = &Kp;
	LVDTController.x = &setPoint;
	LVDTController.y = ulLVDT;
	LVDTController.u = LVDTPIDOutput;

}

void PIDIntHandlerLVDT(void) {
	long P = 0;

	//Compute the error
	error = (*LVDTController.y) - (*LVDTController.x);

	//Compute the P portion of the controller
	P = (*LVDTController.Kp)*error;

	//Compute the D portion of the controller

}
