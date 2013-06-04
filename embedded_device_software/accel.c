/*
 * accel.c
 *
 *  Created on: May 15, 2013
 *      Author: Paul Bartell
 *      Connects to an accelerometer and fetches data at 1kHz, storing in a global variable.
 */

#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "accel.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"

#define NUM_SSI_DATA 2
#define CTRL_REG1 0x20 //0b00111111
#define CTRL_REG4 0x23 //0b00110000
#define XL 0x28
#define XH 0x29
#define YL 0x2a
#define YH 0x2b
#define ZL 0x2c
#define ZH 0x2d
#define WRITE 0x80
#define READ 0x00
#define INCREMENT 0x40

signed long accel;
unsigned long ulDataRx[2];

void accelInit(void){

    //
    // Enable Peripheral Clocks
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable port PA4 for SSI0 SSI0RX
    //
    MAP_GPIOPinConfigure(GPIO_PA4_SSI0RX);
    MAP_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_4);

    //
    // Enable port PA2 for SSI0 SSI0CLK
    //
    MAP_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    MAP_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2);

    //
    // Enable port PA3 for SSI0 SSI0FSS
    //
    MAP_GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    MAP_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_3);

    //
    // Enable port PA5 for SSI0 SSI0TX
    //
    MAP_GPIOPinConfigure(GPIO_PA5_SSI0TX);
    MAP_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5);


	// Assume SSI initialization is taken care of in the pins file.
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
	                       SSI_MODE_MASTER, 10000000, 8); // Set clock 10MHz and frame length (8 bits)
	SSIEnable(SSI0_BASE);
	// Initialize accelerometer module on SSI0 using SPI interface

    while(SSIDataGetNonBlocking(SSI0_BASE, &ulDataRx[0]))
    {
    }

    // Setup Accelerometer

    SSIDataPut(SSI0_BASE, (WRITE | CTRL_REG4));
    SSIDataPut(SSI0_BASE, 0x30);
    SSIDataPut(SSI0_BASE, (WRITE | CTRL_REG1));
    SSIDataPut(SSI0_BASE, 0x3F);
    //Enable with 1kHz data rate
}

void accelInt(void){
	SSIDataPut(SSI0_BASE, READ | XL | INCREMENT);
	SSIDataGet(SSI0_BASE, &ulDataRx[0]);
	SSIDataGet(SSI0_BASE, &ulDataRx[1]);

	accel = (ulDataRx[0] << 8 )| (ulDataRx[1]);

}

