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
#include "newpins.h"
#include "utils/uartstdio.h"
#include "ADCSetup.h"
#include "PWMSetup.h"

//Global variables
unsigned long ulCurrent[1] = {0};				//Stores data read from ADC0 FIFO
unsigned long ulLVDT[1] = {0};				    //Stores data read from ADC1 FIFO
unsigned long dutyCycle = 0;					//Duty cycle for the PWM
unsigned long pwmPeriod = 0;					//Period for the PWM

int main(void) {
	//Enable all required peripherals
	PortFunctionInit();

	//Enable console communication with UART
	UARTStdioInit(0);

	//Set system clock to 40 MHz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    pwmSetup();

    pwmSetDuty(dutyCycle);

	ADCSetup();

	while(1) {
		//SysCtlDelay(SysCtlClockGet() / (1000*3));
		UARTprintf("Current: %u ", ulCurrent[0]);
		UARTprintf("LVDT: %u\n", ulLVDT[0]);
	}

}
