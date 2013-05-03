/*
 * PWMSetup.c
 * Author: Paul Bartell
 * Project: Tissue Tester
 *
 * This file includes the definitions of the functions which are called in the main tissue tester program to enable and configure
 * the PWM as well as TIMER1 on the Stellaris LM4F120XL.
 *
 * Note: Both pwmSetup and pwmSetduty need to be properly called for the PWM to function.
 *
 */

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "newpins.h"
#include "utils/uartstdio.h"
#include "PWMSetup.h"

#define PWMPERIOD 1024

extern unsigned long dutyCycle;
extern unsigned long pwmPeriod;

// Sets up our timer for PWM output
void pwmSetup() {
	// PWM code derived from http://codeandlife.com/2012/10/30/stellaris-launchpad-pwm-tutorial/
	TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM); // set as pwm type

	pwmPeriod = PWMPERIOD; // Set the period to user-defined value

	TimerLoadSet(TIMER1_BASE, TIMER_A, pwmPeriod - 1);
	TimerMatchSet(TIMER1_BASE, TIMER_A, 0); // set duty cycle to 0
	TimerEnable(TIMER1_BASE, TIMER_A);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}

// Sets our duty cycle
void pwmSetDuty(long duty) {

	if (duty > 0){ // positive
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 255);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0);
		TimerMatchSet(TIMER1_BASE, TIMER_A, pwmPeriod - duty - 1);
	} else if(duty < 0){ // negative
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 255);
		TimerMatchSet(TIMER1_BASE, TIMER_A, pwmPeriod + duty -2);
	}else{ // Motor Stop
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 255);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 255);
		TimerMatchSet(TIMER1_BASE, TIMER_A, pwmPeriod - 2);
	}
}
