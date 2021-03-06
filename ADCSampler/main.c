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
unsigned long ulCurrent[1] = {0};				//Stores data read from ADC0 FIFO
unsigned long ulLVDT[1] = {0};				//Stores data read from ADC1 FIFO

int main(void) {
	unsigned long ulPeriod = 0;				//Period for Timer0

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
		ulPeriod = (SysCtlClockGet() / 10) / 2;
		TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod - 1);

	/*
	 *
	 * ADC0 Configuration
	 *
	 */

		//Set the ADC sample rate to 500 KSPS
		SysCtlADCSpeedSet(SYSCTL_ADCSPEED_500KSPS);

		//Disable ADC0 sequencer 3 (so that we can configure it)
		ADCSequenceDisable(ADC0_BASE, 3);
		//Disable ADC1 sequencer 3 (so that we can configure it)
		ADCSequenceDisable(ADC1_BASE, 3);

		//Configure ADC0 sequencer 3 to trigger based on TIMER0A
		ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);
		//Configure ADC1 sequencer 3 to trigger based on TIMER0A
		ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_TIMER, 0);

		//Configure the ADC0 to flag the interrupt flag when it finishes sampling
		ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0|ADC_CTL_END|ADC_CTL_IE);
		//Configure the ADC1 to flag the interrupt flag when it finishes sampling
		ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH1|ADC_CTL_END|ADC_CTL_IE);

		//Enable ADC0 sequencer 3
		ADCSequenceEnable(ADC0_BASE, 3);
		//Enable ADC1 sequencer 3
		ADCSequenceEnable(ADC1_BASE, 3);

		//Enable ADC0 interrupt, it's redundant with line 80 but has to be done
		ADCIntEnable(ADC0_BASE, 3);
		//Enable ADC1 interrupt, it's redundant with line 89 but has to be done
		ADCIntEnable(ADC1_BASE, 3);

		//Enable timer
		TimerEnable(TIMER0_BASE, TIMER_A);

		//Configure TIMER0A to be the ADC sample trigger
		TimerControlTrigger(TIMER0_BASE, TIMER_A, true);

		//Clear any interrupts
		ADCIntClear(ADC0_BASE, 3);
		ADCIntClear(ADC1_BASE, 3);

		//Begin sampling
		ADCIntEnable(ADC0_BASE, 3);
		ADCIntEnable(ADC1_BASE, 3);

		//Turn on ADC0 sequence interrupts for sequence 3
		IntEnable(INT_ADC0SS3);
		//Turn on ADC1 sequence interrupts for sequence 3
		IntEnable(INT_ADC1SS3);

		UARTprintf("ADC0 Configured\n");
		UARTprintf("ADC1 Configured\n");

		//Enable all interrupts
		IntMasterEnable();

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

	UARTprintf("Current: %u ", ulCurrent[0]);

}

/*
 * This function is the implementation of the ADC1 interrupt handler.
 */
interrupt void ADC1IntHandler(void) {
	//Clear the ADC interrupt flags
	ADCIntClear(ADC1_BASE, 3);

	//Get data from ADC0 sequence 3
	ADCSequenceDataGet(ADC1_BASE, 3, ulLVDT);

	UARTprintf("LVDT: %u\n", ulLVDT[0]);

}

