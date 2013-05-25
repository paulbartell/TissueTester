/*
 * app_commands.c
 *
 *  Created on: May 4, 2013
 *      Author: Yan
 */

#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "utils/cmdline.h"
#include "app_commands.h"
#include "PID.h"
#include "PWMSetup.h"

#define BACKSPACE 8

extern PID LVDTController;
extern unsigned long freq;

tCmdLineEntry g_sCmdTable[] =
{
		{"help",	CMD_help,	" : Display list of commands" },
		{"sT",	CMD_setTrajectory,	" : Set the trajectory"	},
		{"start", CMD_start, " : Start the device"},
		{"stop", CMD_stop, " : Stop the device"},
		{"sD", CMD_setDuty, " : Set the duty cycle of the PWM" },
		{"sF", CMD_setFreq, " : Set the frequency" },
		{ 0, 0, 0}
};

const int NUM_CMD = sizeof(g_sCmdTable)/sizeof(tCmdLineEntry);

/*
 * Command: help
 *
 * Print the help strings for all commands.
 */
int CMD_help(int argc, char **argv) {
	int index;

	(void)argc;
	(void)argv;

	UARTprintf("\n");
    for (index = 0; index < NUM_CMD; index++)
    {
      UARTprintf("%17s %s\n\n",
        g_sCmdTable[index].pcCmd,
        g_sCmdTable[index].pcHelp);
    }
    UARTprintf("\n");

    return (0);
}

/*
 * Command: set_Kp_LVDT
 *
 * Allows user to set the value for Kp in the LVDT controller
 */
//int CMD_setKpLVDT(int argc, char **argv) {
//	float newKp = 0.0;
//
//	if (argc == 2) {
//		newKp = strToFloat(argv[1]);
//		*LVDTController.Kp = newKp;
//	}
//	return 0;
//}

/*
 * Command: set_Ki_LVDT
 *
 * Allows user to set the value for Ki in the LVDT controller
 */
//int CMD_setKiLVDT(int argc, char **argv) {
//	float newKi = 0.0;
//
//	if (argc == 2) {
//		newKi = strToFloat(argv[1]);
//		*LVDTController.Ki = newKi;
//	}
//	return 0;
//}

/*
 * Command: set_Kd_LVDT
 *
 * Allows user to set the value for Kd in the LVDT controller
 */
//int CMD_setKdLVDT(int argc, char **argv) {
//	float newKd = 0.0;
//
//	if (argc == 2) {
//		newKd = strToFloat(argv[1]);
//		*LVDTController.Kd = newKd;
//	}
//	return 0;
//}


/*
 * Command: set_trajectory
 *
 * Allows the user to set the trajectory of the indenter. The input trajectory
 * must be between 0 and 4095.
 */
int CMD_setTrajectory(int argc, char **argv) {
	long newTrajectory = 0;

	if (argc == 2) {
		newTrajectory = ustrtoul(argv[1], 0, 10);
		if (newTrajectory < 0 || newTrajectory > 4095) {
			UARTprintf("Invalid trajectory range!\n");
			return 0;
		}
		*LVDTController.x = newTrajectory;
	}
	return 0;
}

/*
 * Command: start
 *
 * Allows the user to start the device with a single command.
 */
int CMD_start(int argc, char **argv) {
	IntEnable(INT_TIMER2A);

	return 0;
}

/*
 * Command: stop
 *
 * Allows the user to stop the device with a single command.
 */
int CMD_stop(int argc, char **argv) {
	IntDisable(INT_TIMER2A);

	return 0;
}

/*
 * Command: setDuty
 *
 * Allows the user to set the duty cycle of the device with a single command.
 */
int CMD_setDuty(int argc, char **argv) {
	long newDuty = 0;

	if(argc == 2) {
		newDuty = ustrtoul(argv[1],0,10);
		if(newDuty < 0 || newDuty > 1024) {
			UARTprintf("Invalid duty value!\n");
			return 0;
		}
		pwmSetDuty(newDuty);
	}
	return 0;
}

int CMD_setFreq(int argc, char **argv) {
	long newFreq = 0;

	if(argc == 2) {
		newFreq = ustrtoul(argv[1],0,10);
		if(newFreq < 0) {
			UARTprintf("Invalid frequency!\n");
			return 0;
		}
		CMD_stop(0, 0);
		freq = newFreq;
		CMD_start(0, 0);
	}
	return 0;
}
/*
 * Helper function strToFloat converts strings to floats.
 *
 */
//float strToFloat(char *arg) {
//	float result = 0.0;
//	float temp1 = 0.0;
//	int i = 0;
//	int j = 0;
//	int power = 0;
//
//	while(*arg != '.' && *arg != 0) {
//		if (arg[i] == BACKSPACE) {
//			continue;
//		}
//		i++;
//		arg++;
//	}
//
//	arg = &arg[0];
//	power = i-1;
//	for (j = 0; j < i; j++) {
//		if (*arg == BACKSPACE) {
//			continue;
//		}
//		temp1 += (*arg - '0')*10^(power);
//		power--;
//	}
//
//	while(*arg != 0) {
//		if (*arg == BACKSPACE) {
//			continue;
//		}
//			i++;
//	}
//
//	power = 1;
//	for (j = j+1; j < i; j++) {
//		if (arg[j] == BACKSPACE) {
//			continue;
//		}
//		result += ((float)(arg[j] - '0'))/((float)(10^power));
//		power++;
//	}
//
//	return result;
//}

