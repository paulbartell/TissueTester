/*
 * structs.h
 *
 *  Created on: Mar 1, 2013
 *      Author: Paul Bartell
 * Description: Packet structs.. Should be shared between client app and microcontroller app.
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_
// Command struct. From master to slave
typedef struct {
	char startbyte;
	char command;
	char datalength;
} commandpacket;

// simple struct with 64 bytes of data.
typedef struct {
	char data[64];
} commanddata;

typedef struct {
	unsigned short trajectory[100];
} trajectory;

// WARNING.. Length will always be a multiple of 4 bytes (32 bits), up to a max of 64 bytes
typedef struct { 				// All units are in ADC counts unless otherwise specified.
	unsigned char startbyte; 	// Packet version.. this is version 1.
	unsigned long time; 		// current time of packet... from RTC module on steallris
	unsigned long lvdt; 		// True position from LVDT sensor
	unsigned long current; 		// measured motor current
	unsigned long accelx;		// X-acceleration (from accelerometer)
	unsigned long cmdval; 		// commanded value (with offset if applicable)
	unsigned long dutycycle;	// last pwm duty cycle value.. if using pwm control.
	unsigned long padding; 		// Easier if we are the right size (32 bytes)
} datapacket;

#endif /* STRUCTS_H_ */

