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

#define NUM_SSI_DATA 16

extern long Accel;
unsigned long ulDataTx[NUM_SSI_DATA];
unsigned long ulDataRx[NUM_SSI_DATA];
unsigned long ulindex;

void accelInit(){
	// Assume ssi initialization is take care of in the pins file.

	// Initialize accelerometer module on SSI0 using SPI interface

    while(SSIDataGetNonBlocking(SSI0_BASE, &ulDataRx[0]))
    {
    }

    ulDataTx[0] = 0x00;

	// Initialize Timer


}

interrupt void accelInt(void){


}

long twoscomp2long(char input[]){

}
