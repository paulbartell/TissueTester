/*
 * main.c
 *
 * Created on: Mar 1, 2013
 *    Authors: Paul Bartell,
 *
 *
 */
#include "structs.h"
#include "main.h"
#include "driverlib/pin_map.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"

unsigned long dutyCycle, pwmPeriod;

void main(void) {

    // 40 MHz system clock
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|
    SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
}
// Sets up our timer for PWM output
void pwmSetup() {
	// PWM code derived from http://codeandlife.com/2012/10/30/stellaris-launchpad-pwm-tutorial/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // enable timer0
	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM); // set as pwm type

	dutyCycle = 250; // 75 % duty cycle (0.25 * ulPeriod)

	TimerLoadSet(TIMER0_BASE, TIMER_A, pwmPeriod - 1);
	// TimerMatchSet(TIMER0_BASE, TIMER_A, pwmPeriod / 2);
	TimerEnable(TIMER0_BASE, TIMER_A);
}

// sets our duty cycle
void pwmSetDuty(unsigned long duty) {
	dutyCycle = pwmPeriod - duty;
	TimerMatchSet(TIMER0_BASE, TIMER_A, duty);
}
