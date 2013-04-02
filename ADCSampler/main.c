#define PART_LM4F120H5QR

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
#include "pins.h"
#include "utils/uartstdio.h"


//Global variables
unsigned long ulCurrent[1];				//Stores data read from ADC0 FIFO
//unsigned long ulLVDT[1];				//Stores data read from ADC1 FIFO

int main(void) {
	unsigned long ulPeriod;				//Period for Timer0

	//Enable all required peripherals
	PortFunctionInit();

	//Enable console communication with UART
	UARTStdioInit(0);

	//Set system clock to 40 MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	/*
	 *
	 * Timer Configuration
	 *
	 */

		//Configure timer to be periodic (counts down and then resets)
		TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

		//Calculate period for a pin toggle freq of 1MHz with 50% duty cycle
		ulPeriod = (SysCtlClockGet() / 100) / 2;
		TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod - 1);

	/*
	 *
	 * ADC Configuration
	 *
	 */

		//Set the ADC sample rate to 500 KSPS
		SysCtlADCSpeedSet(SYSCTL_ADCSPEED_500KSPS);

		//Disable ADC0 sequencer 3 (so that we can configure it)
		ADCSequenceDisable(ADC0_BASE, 3);

		//Configure ADC0 sequencer 3 to trigger based on TIMER0A
		ADCSequenceConfigure(ADC_BASE, 3, ADC_TRIGGER_TIMER, 0);

		//Configure the ADC0 to flag the interrupt flag when it finishes sampling
		ADCSequenceStepConfigure(ADC_BASE, 3, 0, ADC_CTL_CH0|ADC_CTL_END|ADC_CTL_IE);

		//Enable ADC0 sequencer 3
		ADCSequenceEnable(ADC0_BASE, 3);

		//Enable ADC0 interrupt, it's redundant with line 80 but has to be done
		ADCIntEnable(ADC0_BASE, 3);

		//Enable timer
		TimerEnable(TIMER0_BASE, TIMER_A);

		//Configure TIMER0A to be the ADC sample trigger
		TimerControlTrigger(TIMER0_BASE, TIMER_A, true);

		//Clear any interrupts
		ADCIntClear(ADC0_BASE, 1);

		//Begin sampling
		ADCIntEnable(ADC0_BASE, 1);

		//Turn on sequence interrupts for sequence 3
		IntEnable(INT_ADC0SS3);

		//Enable all interrupts
		IntMasterEnable();

		UARTprintf("Configure complete\n");


	while(1) {

	}

}

/*
 * This function is the implementation of the ADC0 interrupt handler.
 */
interrupt void ADC0IntHandler(void) {

	//Clear the ADC interrupt flags
	ADCIntClear(ADC0_BASE, 3);

	//Get data from ADC0 sequence 3
	ADCSequenceDataGet(ADC0_BASE, 3, ulCurrent);

	UARTprintf("Current: %u\n", ulCurrent[0]);

}

