
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "usblib/usblib.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdbulk.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
#include "usb_bulk_structs.h"
#include "ADCSetup.h"
#include "structs.h"
#include "PWM.h"
#include "states.h"

unsigned long ulCurrent[1];
unsigned long ulLVDT[1];
extern unsigned long state;

datapacket packet = {0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//*****************************************************************************
//
// The system tick rate expressed both as ticks per second and a millisecond
// period.
//
//*****************************************************************************
#define SYSTICKS_PER_SECOND     1000
#define SYSTICK_PERIOD_MS       (1000 / SYSTICKS_PER_SECOND)

//*****************************************************************************
//
// The global system tick counter.
//
//*****************************************************************************
volatile unsigned long g_ulSysTickCount = 0;

//*****************************************************************************
//
// Variables tracking transmit and receive counts.
//
//*****************************************************************************
volatile unsigned long g_ulTxCount = 0;
volatile unsigned long g_ulRxCount = 0;
#ifdef DEBUG
unsigned long g_ulUARTRxErrors = 0;
#endif
//*****************************************************************************
//
// Compile out all debug print calls in release builds.
//
//*****************************************************************************
#define DEBUG_PRINT while(0) ((int (*)(char *, ...))0)

//*****************************************************************************
//
// Flags used to pass commands from interrupt context to the main loop.
//
//*****************************************************************************
#define COMMAND_PACKET_RECEIVED 0x00000001
#define COMMAND_STATUS_UPDATE   0x00000002

volatile unsigned long g_ulFlags = 0;
char *g_pcStatus;

//*****************************************************************************
//
// Global flag indicating that a USB configuration has been set.
//
//*****************************************************************************
static volatile tBoolean g_bUSBConfigured = false;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
    UARTprintf("Error at line %d of %s\n", ulLine, pcFilename);
    while(1)
    {
    }
}
#endif


//*****************************************************************************
//
// Receive new data and echo it back to the host.
//
// \param psDevice points to the instance data for the device whose data is to
// be processed.
// \param pcData points to the newly received data in the USB receive buffer.
// \param ulNumBytes is the number of bytes of data available to be processed.
//
// This function is called whenever we receive a notification that data is
// available from the host. We read the data, byte-by-byte and swap the case
// of any alphabetical characters found then write it back out to be
// transmitted back to the host.
//
// \return Returns the number of bytes of data processed.
//
//*****************************************************************************

void SendPacket()
{
	// Pack and send the packet struct.
	packet.time = g_ulSysTickCount;
	packet.lvdt = ulLVDT[0];
	packet.current = ulCurrent[0];
	packet.accelx = 0x00;
	packet.cmdval = 0x00;
	packet.dutycycle = 0x00;
	USBBufferWrite(&g_sTxBuffer,&packet,sizeof(packet));
}
//*****************************************************************************
//
// Interrupt handler for the system tick counter.
//
//*****************************************************************************
void
SysTickIntHandler(void)
{
    //
    // Update our system tick counter.
    //
    g_ulSysTickCount++;
    if(g_bUSBConfigured){
    	SendPacket();
    }
}

//*****************************************************************************
//
// Handles bulk driver notifications related to the transmit channel (data to
// the USB host).
//
// \param pvCBData is the client-supplied callback pointer for this channel.
// \param ulEvent identifies the event we are being notified about.
// \param ulMsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the bulk driver to notify us of any events
// related to operation of the transmit data channel (the IN channel carrying
// data to the USB host).
//
// \return The return value is event-specific.
//
//*****************************************************************************
unsigned long
TxHandler(void *pvCBData, unsigned long ulEvent, unsigned long ulMsgValue,
          void *pvMsgData)
{
    //
    // We are not required to do anything in response to any transmit event
    // in this example. All we do is update our transmit counter.
    //
    if(ulEvent == USB_EVENT_TX_COMPLETE)
    {
        g_ulTxCount += ulMsgValue;
    }

    //
    // Dump a debug message.
    //
    DEBUG_PRINT("TX complete %d\n", ulMsgValue);

    return(0);
}

//*****************************************************************************
//
// Handles bulk driver notifications related to the receive channel (data from
// the USB host).
//
// \param pvCBData is the client-supplied callback pointer for this channel.
// \param ulEvent identifies the event we are being notified about.
// \param ulMsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the bulk driver to notify us of any events
// related to operation of the receive data channel (the OUT channel carrying
// data from the USB host).
//
// \return The return value is event-specific.
//
//*****************************************************************************
unsigned long
RxHandler(void *pvCBData, unsigned long ulEvent,
               unsigned long ulMsgValue, void *pvMsgData)
{
    //
    // Which event are we being sent?
    //
    switch(ulEvent)
    {
        //
        // We are connected to a host and communication is now possible.
        //
        case USB_EVENT_CONNECTED:
        {
            g_bUSBConfigured = true;
            
            UARTprintf("Host connected.\n");

            //
            // Flush our buffers.
            //
            USBBufferFlush(&g_sTxBuffer);
            USBBufferFlush(&g_sRxBuffer);
            state = STATE_CONNECTED;

            break;
        }

        //
        // The host has disconnected.
        //
        case USB_EVENT_DISCONNECTED:
        {
            g_bUSBConfigured = false;
            UARTprintf("Host disconnected.\n");
            state = STATE_IDLE;
            break;
        }

        //
        // A new packet has been received.
        //
        case USB_EVENT_RX_AVAILABLE:
        {
            tUSBDBulkDevice *psDevice;

            //
            // Get a pointer to our instance data from the callback data
            // parameter.
            //
            psDevice = (tUSBDBulkDevice *)pvCBData;
            
            // We need to process command packets here..

            //
            // Read the new packet and echo it back to the host.
            //
            return(0);
        }

        //
        // Ignore SUSPEND and RESUME for now.
        //
        case USB_EVENT_SUSPEND:
        case USB_EVENT_RESUME:
        {
            break;
        }

        //
        // Ignore all other events and return 0.
        //
        default:
        {
            break;
        }
    }

    return(0);
}

void sizes(){
	unsigned short sh;
	unsigned long ln;
	unsigned char ch;
	
	UARTprintf("Short is %d\n",sizeof(sh));
	UARTprintf("long is %d\n",sizeof(ln));
	UARTprintf("Char is %d\n",sizeof(ch));
}
//*****************************************************************************
//
// This is the main application entry function.
//
//*****************************************************************************
int
main(void)
{
    volatile unsigned long ulLoop;
    unsigned long ulTxCount;
    unsigned long ulRxCount;

    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPULazyStackingEnable();

    //
    // Set the clocking to run from the PLL at 80MHz
    // Change _2_5 to _4 for 50MHz
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    //
    // Configure the relevant pins such that UART0 owns them.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable the GPIO pins for the LED (PF2 & PF3).  
    //
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2);

    
    //
    // Open UART0 and show the application name on the UART.
    //
    UARTStdioInit(0);
    
    UARTprintf("TissueTester Debug Interface\n");
    UARTprintf("---------------------------------\n\n");



    //
    // Not configured initially.
    //
    g_bUSBConfigured = false;

    //
    // Enable the GPIO peripheral used for USB, and configure the USB
    // pins.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    
    //
    // Enable the system tick.
    //
    ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / SYSTICKS_PER_SECOND);
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();

    //
    // Tell the user what we are up to.
    //
    UARTprintf("Configuring USB\n");

    //
    // Initialize the transmit and receive buffers.
    //
    USBBufferInit((tUSBBuffer *)&g_sTxBuffer);
    USBBufferInit((tUSBBuffer *)&g_sRxBuffer);


    //
    // Set the USB stack mode to Device mode with VBUS monitoring.
    //
    USBStackModeSet(0, USB_MODE_FORCE_DEVICE, 0);
	

    //
    // Pass our device information to the USB library and place the device
    // on the bus.
    //
    USBDBulkInit(0, (tUSBDBulkDevice *)&g_sBulkDevice);
    //
    // Wait for initial configuration to complete.
    //
    UARTprintf(" USB Initialized.. Waiting for Host\n");

	UARTprintf("Configuring ADC\n");
	ADCSetup();
	
	UARTprintf("Configuring PWM\n");
	pwmSetup();
	
    //
    // Clear our local byte counters.
    //
    ulRxCount = 0;
    ulTxCount = 0;
    
    sizes(); // Display the number of bytes each type takes up.

    //
    // Main application loop.
    //
    while(1)
    {
        //
        // See if any data has been transferred.
        //
        if((ulTxCount != g_ulTxCount) || (ulRxCount != g_ulRxCount))
        {
            //
            // Has there been any transmit traffic since we last checked?
            //
            if(ulTxCount != g_ulTxCount)
            {
                //
                // Turn on the Green LED.
                //
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

                //
                // Delay for a bit.
                //
                for(ulLoop = 0; ulLoop < 150; ulLoop++)
                {
                }
            
                //
                // Turn off the Green LED.
                //
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
                
                //
                // Take a snapshot of the latest transmit count.
                //
                ulTxCount = g_ulTxCount;
            }

            //
            // Has there been any receive traffic since we last checked?
            //
            if(ulRxCount != g_ulRxCount)
            {
                //
                // Turn on the Blue LED.
                //
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

                //
                // Delay for a bit.
                //
                for(ulLoop = 0; ulLoop < 150000; ulLoop++)
                {
                }
            
                //
                // Turn off the Blue LED.
                //
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

                //
                // Take a snapshot of the latest receive count.
                //
                ulRxCount = g_ulRxCount;
            }

            //
            // Update the display of bytes transferred.
            //
            UARTprintf("\rTx: %d  Rx: %d", ulTxCount, ulRxCount);
        }
    }
}
