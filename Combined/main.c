//#define UART_BUFFERED
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
#include "driverlib/uart.h"
#include "inc/hw_uart.h"
#include "ADCSetup.h"
#include "PWMSetup.h"
#include "PID.h"

//Global variables
long ulCurrent[1] = {0};				//Stores data read from ADC0 FIFO
long ulLVDT[1] = {0};				    //Stores data read from ADC1 FIFO
long Accel = 0;						//Stores data read from accelerometer
unsigned long dutyCycle = 0;					//Duty cycle for the PWM
unsigned long pwmPeriod = 0;					//Period for the PWM
long setPoint = 0;
char str [5];
extern PID LVDTController;



int main(void) {
	int count = 0;			//Counter for input loop
	signed long dutynumber = 0;

	//Set system clock to 80 MHz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	//Enable all required peripherals
	PortFunctionInit();

	//Enable console communication with UART
	UARTStdioInit(0);

    pwmSetup();

    //pwmSetDuty(0);

	ADCSetup();

	LVDTPIDInit();

	setPoint = 1000;

	while(1) {
		SysCtlDelay(SysCtlClockGet() / (100*3));

		if (UARTPeek('\r') != -1) {
			UARTgets(str,6);
			dutynumber = 0;
			while (count < 5) {
				if (str[count] >= '0' && str[count] <= '9'){
					dutynumber = dutynumber*10 + (str[count] - '0');
					count++;
				}else if(str[count] == '-'){
					count++;
				}else{
					break;
				}
			}
			if(str[0] == '-'){
				dutynumber = -dutynumber;
				count = 0;
				//pwmSetDuty(dutynumber);
				setPoint = dutynumber;
				//setPoint = 3000;
				UARTprintf("duty set to %d\n", dutynumber);
			}else if(str[0] == 0){
				UARTprintf("C: %u ", ulCurrent[0]);
				UARTFlushTx(0);
				UARTprintf("L: %u ", ulLVDT[0]);
				UARTFlushTx(0);
				UARTprintf("S: %d", setPoint);
				UARTFlushTx(0);
				UARTprintf("u: %d", *LVDTController.u);
				UARTFlushTx(0);
			}else{
				count = 0;
				//pwmSetDuty(dutynumber);
				setPoint = dutynumber;
				//setPoint = 3000;
				UARTprintf("duty set to %d\n", dutynumber);
			}
		}

	}

}
