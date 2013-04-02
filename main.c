/*
 * main.c
 *
 * Created on: Mar 1, 2013
 *    Authors: Paul Bartell,
 *
 *
 */

//#include "datastructs.h"
//#include "main.h"
#include "driverlib/pin_map.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"


unsigned long dutyCycle, pwmPeriod;




void loop() {



}
// Sets up our timer for PWM output
void pwmSetup() {
	// PWM code derived from http://codeandlife.com/2012/10/30/stellaris-launchpad-pwm-tutorial/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // enable timer0
	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM); // set as pwm type

	dutyCycle = 250; // 75 % duty cycle (0.25 * ulPeriod)
	pwmPeriod = 1000;

	TimerLoadSet(TIMER0_BASE, TIMER_A, pwmPeriod - 1);
	TimerMatchSet(TIMER0_BASE, TIMER_A, 0); // set duty cycle to 0
	TimerEnable(TIMER0_BASE, TIMER_A);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 5);

}

// sets our duty cycle
void pwmSetDuty(unsigned long duty) {
	dutyCycle = pwmPeriod - duty;
	TimerMatchSet(TIMER0_BASE, TIMER_A, duty);
}

void updatePID() {

}

void setPosition(){

}

int main(void) {

    // 80 MHz system clock
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);


    // Stuff for PWM initialization
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB6_T0CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);

    pwmSetup();
    pwmSetDuty(250);


    // stuff for UART initialization
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioInit(0);
    UARTprintf("Hello world!\n");


    while(1){
    }
    
    
    return 1;
}

