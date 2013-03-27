#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "pins.h"

int main(void) {
	unsigned long ulADC0Value;			//Stores data read from ADC0 FIFO
	unsigned long ulADC1Value;			//Stores data read from ADC1 FIFO
	unsigned long ulPeriod;				//Period for Timer0

	//Enable all required peripherals
	PortFunctionInit();

	//Set system clock to 40 MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	/*
	 *
	 * ADC Configuration
	 *
	 */
		//Enable Port E
		//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

		//Specify pins 2 and 3 as input pins
		//GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3);

		//Enable ADCO
		//SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

		//Set the ADC sample rate to 1 MSPS
		SysCtlADCSpeedSet(SYSCTL_ADCSPEED_1MSPS);

		//Disable ADC sequencer 4 (so that we can configure it)
		ADCSequenceDisable(ADC0_BASE, 3);

		//Configure ADC sequencer 4 to always trigger
		ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_ALWAYS, 0);

		//Configure the ADC to flag the interrupt flag when it finishes sampling
		ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE);

		//Enable ADC sequencer 4
		ADCSequenceEnable(ADC0_BASE, 3);

		//Enable ADC1
		//SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

		//Set the ADC sample rate to 1 MSPS
		SysCtlADCSpeedSet(SYSCTL_ADCSPEED_1MSPS);

		//Disable ADC sequencer 4 (so that we can configure it)
		ADCSequenceDisable(ADC1_BASE, 3);

		//Configure ADC sequencer 4 to always trigger
		ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_ALWAYS, 0);

		//Configure the ADC to flag the interrupt flag when it finishes sampling
		ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_IE);

		//Enable ADC sequencer 4
		ADCSequenceEnable(ADC1_BASE, 3);

	/*
	 *
	 * Timer and Interrupt Configuration
	 *
	 */



	while(1) {

	}


}
