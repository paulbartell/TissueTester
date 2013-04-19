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
	float Kp;		// Proportional gain
	float Ki;		// Integral gain
	float Kd;		// Derivative gain
	unsigned long *input;	// A pointer to the input of the PID
	unsigned long *output;	// A pointer to the output of the PID
 } PID;
 
<<<<<<< HEAD
 void PIDIntHandlerCurrent(void);

 void PIDIntHandlerLVDT(void);
=======

 void LVDTPIDInit(float, float, float);

 interrupt void PID1IntHandler(void);

 interrupt void PID2IntHandler(void);
>>>>>>> Edited PID.c
 
 #endif // __PID_H__
