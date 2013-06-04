/*
 * states.c
 * Author: Paul Bartell
 * Project: Tissue Tester
 *
 * This file includes the states used by the onboard controller of the tissue tester, 
 * as well as state transition functions.
 *
 */
#include "states.h"
#include "PWM.h"
#include "ADCSetup.h"
#include "PID.h"
#include "trajectory.h"
 
 unsigned long state = STATE_IDLE;
 
 void StateTransition(unsigned long toState){
 	
 	switch(state) {
 	
 		case STATE_IDLE :{
 			// only possible NS is Connected// We should check ESTOP before switching states.
 			pwmEnable();
 			ADCEnable();
 			state = STATE_CONNECTED;
 			break;
 		}
 			
 		case STATE_CONNECTED :
 			switch(toState) {
 				case STATE_IDLE :
 					pwmDisable();
 					ADCDisable();
 					state = STATE_IDLE;
 					break;
 				case STATE_LOAD :
 					//we dont need to do anything
 					state = STATE_LOAD;
 					break;
 				case STATE_TESTING :
 					PIDEnable();
 					trajectoryEnable();
 					state = STATE_TESTING;
 					break;
 				case STATE_ESTOP :
 					PIDDisable();
 					pwmDisable();
 					trajectoryDisable();
 					state = STATE_ESTOP;
 					break;
 				default :
 					//do nothing
 					break;
 			}
 			break;
 			
 		case STATE_LOAD :
 			// we need to switch back to connected state if not estop
 			state = STATE_CONNECTED;
 			break;
 			
 		case STATE_TESTING :
 			switch(toState) {
 				case STATE_CONNECTED :
 					PIDDisable();
 					trajectoryDisable();
 					pwmSetDuty(0);
 					state = STATE_CONNECTED;
 					break;
 				case STATE_ESTOP :
 					//doSomethingElse();
 					break;
 				default :
 					break;
 			}
 			break;
 			
 		case STATE_ESTOP :
 			if(toState == STATE_CONNECTED){
 				//undo estop stuff
 			}
 			break;
 		
 	}
 }
