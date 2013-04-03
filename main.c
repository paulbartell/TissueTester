/*
 * main.c
 *
 * Created on: Mar 1, 2013
 *    Authors: Paul Bartell,
 *
 *
 */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "packets.h"
//#include "main.h"
#include "pins.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"

//Global variables
unsigned long ulCurrent[1];				//Stores data read from ADC0 FIFO
unsigned long ulLVDT[1];				//Stores data read from ADC1 FIFO
unsigned long ulpwmDutyCycle, ulpwmPeriod; 			//Stores PWM Duty cycle value 

// Infinite loop calls this function
void loop() {


}

// Sets up our timer for PWM output
void pwmSetup() {

	// PWM code derived from http://codeandlife.com/2012/10/30/stellaris-launchpad-pwm-tutorial/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB6_T0CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);
    
    
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // enable timer0
	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM); // set as pwm type

	ulpwmDutyCycle = 0; // Start with a 0 duty cycle
	ulpwmPeriod = 1023;

	TimerLoadSet(TIMER0_BASE, TIMER_A, ulpwmPeriod - 1);
	TimerMatchSet(TIMER0_BASE, TIMER_A, ulpwmDutyCycle);
	TimerEnable(TIMER0_BASE, TIMER_A);

}

// sets pwm duty cycle
void pwmSetDuty() {
	TimerMatchSet(TIMER0_BASE, TIMER_A, ulpwmPeriod - ulpwmDutyCycle);
}

void adcSetup(unsigned long adcFrequency) {
	unsigned long ulPeriod; // Temporary variable for adc timer period
	PortFunctionInit();
	/*
	 *
	 * Timer Configuration
	 *
	 */

	//Configure timer to be periodic (counts down and then resets)
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

	//Calculate period for a pin toggle freq of adcFrequency with 50% duty cycle
	ulPeriod = (SysCtlClockGet() / adcFrequency / 2);
	TimerLoadSet(TIMER1_BASE, TIMER_A, ulPeriod - 1);

	/*
	 *
	 * ADC Configuration
	 *
	 */
	
	//Set the ADC sample rate to 500 KSPS
	SysCtlADCSpeedSet(SYSCTL_ADCSPEED_500KSPS);
	//Set ADC oversampling rate. possible are 0,2,4,8,16,32,64
	//ADCHardwareOversampleConfigure(ADC0_BASE, 0);
	//ADCHardwareOversampleConfigure(ADC1_BASE, 0);

	//Disable ADC sequencer 3 (so that we can configure it)
	ADCSequenceDisable(ADC0_BASE, 3);
	ADCSequenceDisable(ADC1_BASE, 3);

	//Configure ADC0 sequencer 3 to trigger based on a timer.
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);
	ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_TIMER, 0);


	//Configure the ADC0 to flag the interrupt flag when it finishes sampling
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0|ADC_CTL_END|ADC_CTL_IE);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0|ADC_CTL_END|ADC_CTL_IE);

	//Enable ADC0 sequencer 3
	ADCSequenceEnable(ADC0_BASE, 3);
	ADCSequenceEnable(ADC1_BASE, 3);

	//Enable ADC0 interrupt, ... seems redundant
	ADCIntEnable(ADC0_BASE, 3);
	ADCIntEnable(ADC1_BASE, 3);

	//Enable timer
	TimerEnable(TIMER1_BASE, TIMER_A);

	//Configure TIMER0A to be the ADC sample trigger
	TimerControlTrigger(TIMER1_BASE, TIMER_A, true);
	TimerControlTrigger(TIMER1_BASE, TIMER_A, true);

	//Clear any interrupts // These are for Squencer 1...???
	ADCIntClear(ADC0_BASE, 1);
	ADCIntClear(ADC1_BASE, 1);

	//Begin sampling
	ADCIntEnable(ADC0_BASE, 1);
	ADCIntEnable(ADC1_BASE, 1);

	//Turn on sequence interrupts for sequence 3
	IntEnable(INT_ADC0SS3);
	IntEnable(INT_ADC1SS3);

	//Enable all interrupts
	IntMasterEnable();

}

void ADC0IntHandler() {

	//Clear the ADC interrupt flags
	ADCIntClear(ADC0_BASE, 3);

	//Get data from ADC0 sequence 3
	ADCSequenceDataGet(ADC0_BASE, 3, ulCurrent);

	UARTprintf("Current: %u\n", ulCurrent[0]);

}

void ADC1IntHandler() {

	//Clear the ADC interrupt flags
	ADCIntClear(ADC1_BASE, 3);

	//Get data from ADC1 sequence 3
	ADCSequenceDataGet(ADC1_BASE, 3, ulLVDT);

	UARTprintf("LVDT: %u\n", ulLVDT[0]);

}

void uartSetup() {

	// stuff for UART initialization
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioInit(0);

}



void updatePID() {

}

void setPosition(){

}

int main(void) {

    // 80 MHz system clock
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    pwmSetup(); // initialize with 0 duty cycle.
    
    pwmSetDuty(250);
    
    uartSetup();
    
    adcSetup(1000000);

	// Set status LEDS
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 5);
    

    UARTprintf("Hello world!\n");


    while(1){
    }
    
    
    return 1;
}

