/*
 * app_commands.h
 *
 *  Created on: May 4, 2013
 *      Author: Yan
 */


#ifndef APP_COMMANDS_H_
#define APP_COMMANDS_H_

#define CMDLINE_MAX_ARGS 3
#define APP_INPUT_BUF_SIZE 128

float strToFloat(char *arg);

extern int CMD_help(int argc, char **argv);
//extern int CMD_setKpLVDT(int argc, char **argv);
//extern int CMD_setKiLVDT(int argc, char **argv);
//extern int CMD_setKdLVDT(int argc, char **argv);
extern int CMD_setTrajectory(int argc, char **argv);
extern int CMD_start(int argc, char **argv);
extern int CMD_stop(int argc, char **argv);
extern int CMD_setDuty(int argc, char **argv);
extern int CMD_setFreq(int argc, char **argv);

#endif /* APP_COMMANDS_H_ */
