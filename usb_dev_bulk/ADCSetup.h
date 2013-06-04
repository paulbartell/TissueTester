/*
 * ADCSetup.h
 * Author: Yan Gouts
 * Project: Tissue Tester
 *
 * This file includes the prototypes which are called in the main tissue tester program to enable and configure the ADC
 * modules as well as TIMER0A on the Stellaris LM4F120XL. This file also includes the prototypes for the handlers of both
 * the ADC modules.
 *
 */

#ifndef __ADCSETUP_H__
#define __ADCSETUP_H__

// Prototype for the function that is called when the user wants to enable and configure the ADC0 and ADC1 modules
// as well as TIMER0A, which triggers the sampling/interrupts for both ADC modules.
void ADCSetup(void);

// Prototype for the interrupt handler for ADC0.
void ADC0IntHandler(void);

// Prototype for the interrupt handler for ADC1.
void ADC1IntHandler(void);

void ADCEnable();
void ADCDisable();

void zeroLVDT(unsigned long);

#endif // __ADCSETUP_H__
