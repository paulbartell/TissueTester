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
unsigned long ulLVDT[1];				//Stores data read from ADC1 FIFO

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
	 * ADC Configuration
	 *
	 */

		//Set the ADC sample rate to 1 MSPS
		SysCtlADCSpeedSet(SYSCTL_ADCSPEED_1MSPS);

		//Disable ADC0 sequencer 4 (so that we can configure it)
		ADCSequenceDisable(ADC0_BASE, 3);

		//Configure ADC0 sequencer 4 to always trigger
		ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

		//Configure the ADC0 to flag the interrupt flag when it finishes sampling
		ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0|ADC_CTL_END|ADC_CTL_IE);

		//Enable ADC0 sequencer 4
		ADCSequenceEnable(ADC0_BASE, 3);

		/*

		//Set the ADC sample rate to 1 MSPS
		SysCtlADCSpeedSet(SYSCTL_ADCSPEED_1MSPS);

		//Disable ADC1 sequencer 4 (so that we can configure it)
		ADCSequenceDisable(ADC1_BASE, 3);

		//Configure ADC1 sequencer 4 to always trigger
		ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

		//Configure the ADC1 to flag the interrupt flag when it finishes sampling
		ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH1|ADC_CTL_END|ADC_CTL_IE);

		//Enable ADC1 sequencer 4
		ADCSequenceEnable(ADC1_BASE, 3);

		*/

	/*
	 *
	 * Timer and Interrupt Configuration
	 *
	 */
		//Configure timer to be periodic (counts down and then resets)
		TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

		//Calculate period for a pin toggle freq of 1MHz with 50% duty cycle
		ulPeriod = (SysCtlClockGet() / 100) / 2;
		TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod - 1);

		//Configure and enable interrupt in timer module as well as NVIC
		IntEnable(INT_TIMER0A);
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		IntMasterEnable();

		//Enable timer
		TimerEnable(TIMER0_BASE, TIMER_A);

		//UARTprintf("Handler called\n");


	while(1) {

	}

}

/*
 * This function is the implementation of the Timer0 interrupt handler.
 */
void Timer0IntHandler(void) {
	//Clear the timer interrupt flag
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	//Clear the ADC interrupt flags
	ADCIntClear(ADC0_BASE, 3);
	//ADCIntClear(ADC1_BASE, 3);

	//Trigger ADC conversion
	ADCProcessorTrigger(ADC0_BASE, 3);
	//ADCProcessorTrigger(ADC1_BASE, 3);

	//Wait for conversions to finish
	while(!ADCIntStatus(ADC0_BASE, 3, false))
	{
	}

	//while(!ADCIntStatus(ADC1_BASE, 3, false))
	//{
	//}

	ADCSequenceDataGet(ADC0_BASE, 3, ulCurrent);
	//ADCSequenceDataGet(ADC1_BASE, 3, ulLVDT);


}

