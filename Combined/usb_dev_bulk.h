/*
 * usb_dev_bulk.h
 *
 *  Created on: May 6, 2013
 *      Author: BRL
 */
volatile unsigned long g_ulSysTickCount = 0;
volatile unsigned long g_ulTxCount = 0;
volatile unsigned long g_ulRxCount = 0;
#ifdef DEBUG
unsigned long g_ulUARTRxErrors = 0;
#endif
volatile unsigned long g_ulFlags = 0;
char *g_pcStatus;
static volatile tBoolean g_bUSBConfigured = false;

void
__error__(char *pcFilename, unsigned long ulLine);
unsigned long TxHandler(void *pvCBData, unsigned long ulEvent, unsigned long ulMsgValue,
          void *pvMsgData);
static unsigned long
EchoNewDataToHost(tUSBDBulkDevice *psDevice, unsigned char *pcData,
                  unsigned long ulNumBytes);
unsigned long
RxHandler(void *pvCBData, unsigned long ulEvent,
               unsigned long ulMsgValue, void *pvMsgData);
unsigned long
TxHandler(void *pvCBData, unsigned long ulEvent, unsigned long ulMsgValue,
          void *pvMsgData);

void
UsbMain(void);
