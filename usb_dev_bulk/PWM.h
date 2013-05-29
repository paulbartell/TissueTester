/*
 * PWM.h
 * Author: Paul Bartell
 * Project: Tissue Tester
 *
 * This file includes the prototypes which are called in the main tissue tester program to enable and configure the PWM
 * as well as TIMER1 on the Stellaris LM4F120XL.
 *
 * Note: Both pwmSetup and pwmSetduty need to be properly called for the PWM to function.
 *
 */

#ifndef __PWM_H_
#define __PWM_H_

// Prototype for the function that is called when the user needs to set up TIMER1 to drive the PWM.
void pwmSetup(void);

// Prototype for the function that is called when the user needs to set up the duty cycle of the PWM.
void pwmSetDuty(long);

void pwmEnable(void);

void pwmDisable(void);

#endif /* __PWM_H_ */
