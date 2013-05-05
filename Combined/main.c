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
#include "driverlib/rom.h"
#include "usblib/usblib.h"
//#include "usblib/device/usbdhandler.c"
//#include "usblib/usb-ids.h"
//#include "usblib/device/usbdevice.h"
//#include "usblib/device/usbdbulk.h"

//SYSTICK stuff
#define SYSTICKS_PER_SECOND     100
#define SYSTICK_PERIOD_MS       (1000 / SYSTICKS_PER_SECOND)
volatile unsigned long g_ulSysTickCount = 0;

//Global variables
long ulCurrent[1] = {0};				//Stores data read from ADC0 FIFO
long ulLVDT[1] = {0};				    //Stores data read from ADC1 FIFO
unsigned long dutyCycle = 0;					//Duty cycle for the PWM
unsigned long pwmPeriod = 0;					//Period for the PWM
long setPoint = 0;
float Kp = 0.05;
float Ki = 7.69/100000;
float Kd = 0;
char str [5];
extern PID LVDTController;



int main(void) {
	int count = 0;			//Counter for input loop
	signed long dutynumber = 0;
	//Enable all required peripherals
	PortFunctionInit();

	ROM_FPULazyStackingEnable();

	//Set system clock to 80 MHz
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / SYSTICKS_PER_SECOND);
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();

	//Enable console communication with UART0
	UARTStdioInit(0);
	UARTprintf("test1");
	//UsbInit();



    pwmSetup();
	UARTprintf("test2");

    //pwmSetDuty(0);

	ADCSetup();
	UARTprintf("test3");

	LVDTPIDInit();
	UARTprintf("test4");

	while(1) {
		UARTprintf("iter");
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
