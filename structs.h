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
    uint8 command;
	uint8 datalength;
} commandpacket;

typedef struct {
	uint8 data[64];
} commanddata;

typedef struct { 		// All units are in ADC counts unless otherwise specified.
	uint8 packetver; 	// Packet version.. this is version 1.
	uint32 time; 		// current time of packet... from RTC module on steallris
	uint16 cmdpos; 		// commanded position (with offset)
	uint16 sensepos; 	// True position from LVDT sensor
	uint16 cmdmotorv; 	// commanded motor voltage
	uint16 sensemotorv; // measured motor voltage
	uint16 sensemotorc; // measured motor current
	uint16 accelx;		// X-acceleration (from accelerometer)
	uint16 accely;		// Y-acceleration
	uint16 accelz;		// Z-acceleration
	uint16 temp;		// Temperature.

} datapacket;

#endif /* STRUCTS_H_ */
