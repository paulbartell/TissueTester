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

extern unsigned long ulCurrent[1];				//Stores data read from ADC0 FIFO
extern unsigned long ulLVDT[1];				    //Stores data read from ADC1 FIFO
PID LVDTController;
unsigned long LVDTPIDOutput[1] = {0};					//Output from the LVDT controller

void LVDTPIDInit(float Kd, float Ki, float Kp) {
	//Initialize the controller's gains as well as input and output
	LVDTController.Kd = Kd;
	LVDTController.Ki = Ki;
	LVDTController.Kp = Kp;
	LVDTController.input = ulLVDT;
	LVDTController.output = LVDTPIDOutput;

}

void PIDIntHandlerLVDT(void) {

}
