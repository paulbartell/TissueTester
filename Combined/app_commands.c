/*
 * app_commands.c
 *
 *  Created on: May 4, 2013
 *      Author: Yan
 */

#include "inc/hw_types.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "utils/cmdline.h"
#include "app_commands.h"
#include "PID.h"

#define BACKSPACE 8

extern PID LVDTController;

tCmdLineEntry g_sCmdTable[] =
{
		{"help",	CMD_help,	" : Display list of commands" },
		{"set_Kp_LVDT",	CMD_set_Kp_LVDT,	" : Set a value for Kp" },
		{"set_Ki_LVDT",	CMD_set_Ki_LVDT,	" : Set a value for Ki" },
		{"set_Kd_LVDT",	CMD_set_Kd_LVDT,	" : Set a value for Kd"	},
		{"set_trajectory",	CMD_set_trajectory,	" : Set the trajectory"	},
		{ 0, 0, "--teriminating entry--" }
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
int CMD_set_Kp_LVDT(int argc, char **argv) {
	float newKp = 0.0;

	if (argc == 2) {
		newKp = strToFloat(argv[1]);
		*LVDTController.Kp = newKp;
	}
	return 0;
}

/*
 * Command: set_Ki_LVDT
 *
 * Allows user to set the value for Ki in the LVDT controller
 */
int CMD_set_Ki_LVDT(int argc, char **argv) {
	float newKi = 0.0;

	if (argc == 2) {
		newKi = strToFloat(argv[1]);
		*LVDTController.Ki = newKi;
	}
	return 0;
}

/*
 * Command: set_Kd_LVDT
 *
 * Allows user to set the value for Kd in the LVDT controller
 */
int CMD_set_Kd_LVDT(int argc, char **argv) {
	float newKd = 0.0;

	if (argc == 2) {
		newKd = strToFloat(argv[1]);
		*LVDTController.Kd = newKd;
	}
	return 0;
}

/*
 * Command: set_trajectory
 *
 * Allows the user to set the trajectory of the indenter. The input trajectory
 * must be between 0 and 4095.
 */
int CMD_set_trajectory(int argc, char **argv) {
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
 * Helper function strToFloat converts strings to floats.
 *
 */
float strToFloat(char *arg) {
	float result = 0.0;
	float temp1 = 0.0;
	int i = 0;
	int j = 0;
	int power = 0;

	while(arg[i] != '.' || arg[i] != '\0') {
		if (arg[i] == BACKSPACE) {
			continue;
		}
		i++;
	}

	power = i-1;
	for (j = 0; j < i; j++) {
		if (arg[j] == BACKSPACE) {
			continue;
		}
		temp1 += (arg[j] - '0')*10^(power);
		power--;
	}

	while(arg[i] != '\0') {
		if (arg[i] == BACKSPACE) {
			continue;
		}
			i++;
	}

	power = 1;
	for (j = j+1; j < i; j++) {
		if (arg[j] == BACKSPACE) {
			continue;
		}
		result += ((float)(arg[j] - '0'))/((float)(10^power));
		power++;
	}

	return result;
}

