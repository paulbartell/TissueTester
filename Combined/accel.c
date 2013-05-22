/*
 * accel.c
 *
 *  Created on: May 15, 2013
 *      Author: Paul Bartell
 *      Connects to an accelerometer and fetched data at 1kHz, storing in a global variable.
 */

#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"

#define NUM_SSI_DATA 2
#define CTRL_REG1 0x20 //0b00111111
#define CTRL_REG4 0x23 //0b00110000
#define XL 0x28
#define XH 0x29
#define YL 0x2a
#define YH 0x2b
#define ZL 0x2c
#define ZH 0x2d
#define WRITE 0b10000000
#define READ 0b00000000
#define INCREMENT 0b01000000



extern long Accel[3];
unsigned long ulDataRx[6];
unsigned long ulindex;

void accelInit(){
	// Assume ssi initialization is take care of in the pins file.
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
	                       SSI_MODE_MASTER, 1000000, 16); // Set clock and frame length (16 bits)
	SSIEnable(SSI0_BASE);
	// Initialize accelerometer module on SSI0 using SPI interface

    while(SSIDataGetNonBlocking(SSI0_BASE, &ulDataRx[0]))
    {
    }

    // Setup Accelerometer

    SSIDataPut(SSI0_BASE, ((WRITE | CTRL_REG4) << 8) | 0b00110000);
    SSIDataPut(SSI0_BASE, ((WRITE | CTRL_REG1) << 8) | 0b00111111);
    //Enable with 1kHz data rate

	// Initialize Timer

    // Change to 8-bit blocks
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
    	                       SSI_MODE_MASTER, 1000000, 8);
}

interrupt void accelInt(void){
	SSIDataPut(SSI0_BASE, ((READ | INCREMENT | XL)));
	SSIDataGet(SSI0_BASE, &ulDataRx[0]);
	SSIDataGet(SSI0_BASE, &ulDataRx[1]);
	SSIDataGet(SSI0_BASE, &ulDataRx[2]);
	SSIDataGet(SSI0_BASE, &ulDataRx[3]);
	SSIDataGet(SSI0_BASE, &ulDataRx[4]);
	SSIDataGet(SSI0_BASE, &ulDataRx[5]);
	& 0xff

}

long twoscomp2long(char input[]){

	return 0;
}
