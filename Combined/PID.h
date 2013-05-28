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
	long *x;			// a pointer to the desired trajectory element
	long *y;			// a pointer to the output of the system
	long yLast;			// the last output of the system (for calculating D term)
	long u;				// the output of the PID
	float sumError; 	// the sum of the error for the integral term
	float maxSumError; 	// the maximum amount of error to be summed
	float Kp;			// Proportional gain
	float Ki;			// Integral gain
	float Kd;			// Derivative gain
 } PID;

void LVDTPIDInit(void);

void IPIDInit(void);

interrupt void PIDIntHandler(void);

unsigned long roundNum(float num);

 
 #endif // __PID_H__
