#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include "utils/cmdline.h"
#include "newpins.h"
#include "driverlib/uart.h"
#include "ADCSetup.h"
#include "PWMSetup.h"
#include "PID.h"
#include "app_commands.h"

//Global variables
long ulCurrent[1] = {0};				//Stores data read from ADC0 FIFO
long ulLVDT[1] = {0};				    //Stores data read from ADC1 FIFO
long Accel = 0;						//Stores data read from accelerometer
unsigned long dutyCycle = 0;					//Duty cycle for the PWM
unsigned long pwmPeriod = 0;					//Period for the PWM
long setPoint = 0;
char str [5];
static char g_cInput[APP_INPUT_BUF_SIZE];	//Input buffer for the command line interpreter.
extern PID LVDTController;


int main(void) {
	// int count = 0;			//Counter for input loop
	// signed long dutynumber = 0;
	long lCommandStatus;

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

	UARTprintf("Welcome to the BRL Tissue Testing interface!\n");
	UARTprintf("Type 'help' for a list of commands\n");
	UARTprintf("> ");

	SysCtlDelay(SysCtlClockGet() / (1000/3));

    IntMasterEnable();
	setPoint = 1000;

	while(1) {
			UARTprintf("\n>");
		/*
		 * Peek to see if a full command is ready for processing
		 */
		while(UARTPeek('\r') == -1) {
			SysCtlDelay(SysCtlClockGet() / (1000/3));
		}

	/*
	 * a '/r' was detected so get the line of text from the user.
	 */
	UARTgets(g_cInput, sizeof(g_cInput));

	/*
	 * Pass the line from the user to the command processor.
	 * It will be parsed and valid commands executed.
	 */
	lCommandStatus = CmdLineProcess(g_cInput);

	/*
	 * Handle the case of bad command.
	 */
	if(lCommandStatus == CMDLINE_BAD_CMD) {
		UARTprintf("Bad command!\n");
	}

	/*
	 * Handle the case of too many arguments
	 */
	else if(lCommandStatus == CMDLINE_TOO_MANY_ARGS) {
		UARTprintf("Too many arguments for command processor!\n");
	}
		// SysCtlDelay(SysCtlClockGet() / (100*3));

		// if (UARTPeek('\r') != -1) {
			// UARTgets(str,6);
			// dutynumber = 0;
			// while (count < 5) {
				// if (str[count] >= '0' && str[count] <= '9'){
					// dutynumber = dutynumber*10 + (str[count] - '0');
					// count++;
				// }else if(str[count] == '-'){
					// count++;
				// }else{
					// break;
				// }
			// }
			// if(str[0] == '-'){
				// dutynumber = -dutynumber;
				// count = 0;
				// //pwmSetDuty(dutynumber);
				// setPoint = dutynumber;
				// //setPoint = 3000;
				// UARTprintf("duty set to %d\n", dutynumber);
			// }else if(str[0] == 0){
				// UARTprintf("C: %u ", ulCurrent[0]);
				// UARTFlushTx(0);
				// UARTprintf("L: %u ", ulLVDT[0]);
				// UARTFlushTx(0);
				// UARTprintf("S: %d", setPoint);
				// UARTFlushTx(0);
				// UARTprintf("u: %d", *LVDTController.u);
				// UARTFlushTx(0);
			// }else{
				// count = 0;
				// //pwmSetDuty(dutynumber);
				// setPoint = dutynumber;
				// //setPoint = 3000;
				// UARTprintf("duty set to %d\n", dutynumber);
			// }
		// }

	}

}
