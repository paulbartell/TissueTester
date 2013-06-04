/*
 * ADCSetup.c
 * Author: Yan Gouts
 * Project: Tissue Tester
 *
 * This file includes the definitions of the functions which are called in the main tissue tester program to enable and configure
 * the ADC modules as well as TIMER0A on the Stellaris LM4F120XL. This file also includes the interrupt handlers for both ADC modules.
 *
 */

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "ADCSetup.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"

#define SEQUENCER 3
#define PRIORITY 0
#define AVGSAMPLES 64

extern long ulCurrent[];
extern long ulLVDT[];
extern long gDuty;
long ADCtemp[2];
long ulLVDTZero;

//Sets up ADC0, ADC1 and TIMER0A to trigger the sampling
void ADCSetup(void) {
	unsigned long ulPeriod = 0;				//Period for Timer0
	
	/*
	 *
	 * Hardware Configuration
	 *
	 */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

    //
    // Enable port PE2 for ADC AIN1
    //
    MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
    //
    // Enable port PE3 for ADC AIN0
    //
    MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    //
    // Enable port PB6 for TIMER0 T0CCP0
    //
    MAP_GPIOPinConfigure(GPIO_PB6_T0CCP0);
    MAP_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);
    //
    // Enable port PB7 for TIMER0 T0CCP1
    //
    MAP_GPIOPinConfigure(GPIO_PB7_T0CCP1);
    MAP_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_7);
	 
	/*
	 *
	 * Timer Configuration
	 *
	 */

		//Configure timer to be periodic (counts down and then resets)
		TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

		//Calculate period for a pin toggle freq of 1kHz with 50% duty cycle
		ulPeriod = (SysCtlClockGet() / 64000) / 2;
		TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod - 1);

	/*
	 *
	 * ADC0 Configuration
	 *
	 */

		//Set the ADC sample rate to 500 KSPS
		SysCtlADCSpeedSet(SYSCTL_ADCSPEED_500KSPS);
		ADCHardwareOversampleConfigure(ADC0_BASE, AVGSAMPLES);
		ADCHardwareOversampleConfigure(ADC1_BASE, AVGSAMPLES);

		//Disable ADC0 sequencer 3 (so that we can configure it)
		ADCSequenceDisable(ADC0_BASE, SEQUENCER);
		//Disable ADC1 sequencer 3 (so that we can configure it)
		ADCSequenceDisable(ADC1_BASE, SEQUENCER);

		//Configure ADC0 sequencer 3 to trigger based on TIMER0A
		ADCSequenceConfigure(ADC0_BASE, SEQUENCER, ADC_TRIGGER_TIMER, PRIORITY);
		//Configure ADC1 sequencer 3 to trigger based on TIMER0A
		ADCSequenceConfigure(ADC1_BASE, SEQUENCER, ADC_TRIGGER_TIMER, PRIORITY);

		//Configure the ADC0 to flag the interrupt flag when it finishes sampling
		ADCSequenceStepConfigure(ADC0_BASE, SEQUENCER, 0, ADC_CTL_CH0|ADC_CTL_END|ADC_CTL_IE);
		//Configure the ADC1 to flag the interrupt flag when it finishes sampling
		ADCSequenceStepConfigure(ADC1_BASE, SEQUENCER, 0, ADC_CTL_CH1|ADC_CTL_END|ADC_CTL_IE);

		//Enable ADC0 sequencer 3
		ADCSequenceEnable(ADC0_BASE, SEQUENCER);
		//Enable ADC1 sequencer 3
		ADCSequenceEnable(ADC1_BASE, SEQUENCER);

		//Enable ADC0 interrupt, it's redundant with line 80 but has to be done
		ADCIntEnable(ADC0_BASE, SEQUENCER);
		//Enable ADC1 interrupt, it's redundant with line 89 but has to be done
		ADCIntEnable(ADC1_BASE, SEQUENCER);

		//Enable timer
		TimerEnable(TIMER0_BASE, TIMER_A);

		//Configure TIMER0A to be the ADC sample trigger
		TimerControlTrigger(TIMER0_BASE, TIMER_A, true);

		//Clear any interrupts
		ADCIntClear(ADC0_BASE, SEQUENCER);
		ADCIntClear(ADC1_BASE, SEQUENCER);

		//Begin sampling
		ADCIntEnable(ADC0_BASE, SEQUENCER);
		ADCIntEnable(ADC1_BASE, SEQUENCER);

		//Turn on ADC0 sequence interrupts for sequence 3
		IntEnable(INT_ADC0SS3);
		//Turn on ADC1 sequence interrupts for sequence 3
		IntEnable(INT_ADC1SS3);

		//Set ADC interrupts to be lower priority than PID
		IntPrioritySet(INT_ADC0SS3, 0x20);
		IntPrioritySet(INT_ADC1SS3, 0x20);

		UARTprintf("ADC0 Configured\n");
		UARTprintf("ADC1 Configured\n");

		//Enable all interrupts
		IntMasterEnable();
}

void ADCEnable(){
	// What should go here?
}
void ADCDisable(){
	// What should go here?
}

/*
 * This function is the implementation of the ADC0 interrupt handler.
 */
void ADC0IntHandler(void) {
	//Clear the ADC interrupt flags
	ADCIntClear(ADC0_BASE, SEQUENCER);

	//Get data from ADC0 sequence 3
	ADCSequenceDataGet(ADC0_BASE, SEQUENCER, &ADCtemp[0]);

	if(gDuty < 0) {
		ulCurrent[0] = -1*ADCtemp[0];
	}
	else {
		ulCurrent[0] = ADCtemp[0];
	}
}

/*
 * This function is the implementation of the ADC1 interrupt handler.
 */
void ADC1IntHandler(void) {
	//Clear the ADC interrupt flags
	ADCIntClear(ADC1_BASE, SEQUENCER);

	//Get data from ADC0 sequence 3
	ADCSequenceDataGet(ADC1_BASE, SEQUENCER, &ADCtemp[1]);
	ulLVDT[0] = ADCtemp[1] - ulLVDTZero;
}

void zeroLVDT(unsigned long zero){
	ulLVDTZero = zero;
}

