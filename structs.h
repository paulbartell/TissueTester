/*
 * structs.h
 *
 *  Created on: Mar 1, 2013
 *      Author: Paul Bartell
 * Description: Packet structs.. Should be shared between client app and microcontroller app.
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_


typedef struct {
	char command;
	char datalength;
} commandpacket;

typedef struct {
	char data[64];
} commanddata;

typedef struct { 		// All units are in ADC counts unless otherwise specified.
	char packetver; 	// Packet version.. this is version 1.
	unsigned long time; 		// current time of packet... from RTC module on steallris
	unsigned long cmdpos; 		// commanded position (with offset)
	unsigned long sensepos; 	// True position from LVDT sensor
	unsigned long dutycycle;	// last pwm duty cycle.. if using pwm control.
	unsigned long cmdmotorv; 	// commanded motor voltage .. only if using DAC for control.
	unsigned long sensemotorv; // measured motor voltage
	unsigned long sensemotorc; // measured motor current
	unsigned long accelx;		// X-acceleration (from accelerometer)
	unsigned long accely;		// Y-acceleration
	unsigned long accelz;		// Z-acceleration
	unsigned long temp;		// Temperature.

} datapacket;

#endif /* STRUCTS_H_ */
