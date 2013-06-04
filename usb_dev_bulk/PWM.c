/*
 * PWM.c
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
#include "utils/uartstdio.h"
#include "PWM.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"

#define PWMPERIOD 1024

unsigned long pwmPeriod;
long gDuty = 0;

// Sets up our timer for PWM output
void pwmSetup() {

	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_7);
    	
	//
    // Enable port PF3 for TIMER1 T1CCP1
    //
    MAP_GPIOPinConfigure(GPIO_PF3_T1CCP1);
    MAP_GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_3);

    //
    // Enable port PF2 for TIMER1 T1CCP0
    //
    MAP_GPIOPinConfigure(GPIO_PF2_T1CCP0);
    MAP_GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_2);


	// PWM code derived from http://codeandlife.com/2012/10/30/stellaris-launchpad-pwm-tutorial/
	TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM); // set as pwm type

	pwmPeriod = PWMPERIOD; // Set the period to user-defined value

	TimerLoadSet(TIMER1_BASE, TIMER_A, pwmPeriod - 1);
	TimerMatchSet(TIMER1_BASE, TIMER_A, pwmPeriod - 2); // set duty cycle to 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}
// Enables pwm control
void pwmEnable(void){
	TimerEnable(TIMER1_BASE, TIMER_A);
}

// Disables PWM timer
void pwmDisable(void){
	TimerDisable(TIMER1_BASE, TIMER_A);
}

// Sets our duty cycle
void pwmSetDuty(long duty) {
	gDuty = duty;

	if (gDuty > 0){ // positive
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 255);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0);
		TimerMatchSet(TIMER1_BASE, TIMER_A, pwmPeriod - gDuty - 1);
	} else if(gDuty < 0){ // negative
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 255);
		TimerMatchSet(TIMER1_BASE, TIMER_A, pwmPeriod + gDuty -2);
	}else{ // Motor Stop
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 255);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 255);
		TimerMatchSet(TIMER1_BASE, TIMER_A, pwmPeriod - 2);
	}
}
