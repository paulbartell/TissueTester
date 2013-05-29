/*
 * states.h
 * Author: Paul Bartell
 * Project: Tissue Tester
 *
 * This file includes the states used by the onboard controller of the tissue tester.
 *
 */
 
 #define STATE_IDLE 0x00
 #define STATE_CONNECTED 0x01
 #define STATE_TESTING 0x02
 #define STATE_LOAD 0x03
 #define STATE_ESTOP 0x04
 
 void StateTransition(unsigned long toState);
 unsigned long state;