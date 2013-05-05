/*
 * PID.h
 * Author: Yan Gouts
 * Project: Tissue Tester
 *
 * This file includes the prototypes for the functions related to the PID controller which are called in the main.c 
 * of the Tissue Tester project. This file also defines the struct pid along with the ISRs related to the PID.
 *
 */
 
 #ifndef __PID_H__
 #define __PID_H__
 
typedef struct PID {
	float* Kp;			// Proportional gain
	float* Ki;			// Integral gain
	float* Kd;			// Derivative gain
	long* x;			// A pointer to the desired trajectory element
	long* y;			// A pointer to the output of the system
	long* yLast;		// A pointer to the last output of the system (for calculating D term)
	long* u;			// A pointer to the output of the PID
	float* sumError; 	// A pointer to the sum of the error for the integral term
	float* maxSumError; // A pointer to the maximum amount of error to be summed.
 } PID;

void LVDTPIDInit(void);

interrupt void PIDIntHandlerLVDT(void);

unsigned long round(float num);

 
 #endif // __PID_H__
