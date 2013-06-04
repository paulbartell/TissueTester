# Tissue Tester Embedded C Code

This embedded code package provides the controller and communication for the TissueTester.

## Main File: usb_dev_bulk.c
The embedded software code is loosely based upon TI's EK-LM4F120XL Firmware Package usb_dev_bulk Example code. This file is the main file and sets up the various modules of the device.

## states.c
This file provides the state-machine functionality of the controller. More information on the states it uses is available at the ee400b wiki.

## PID.c
This file implements two PID controllers for controlling the actuator.

## ADCSetup.c
This file provides functions that set up the two ADCs and provides ISRs for both ADCS, storing and signing the sampled data.

## PWM.c
This file sets up the PWM functionality using Timer 1. It allows setting a duty cycle using the pwmSetDuty function.

## accel.c
This is an experimental interface with an LIS331 accelerometer module.

## buttons.c
This file provides a driver for the buttons. This allows some amount of an Emergency stop functionality.

## startupXX.c
These files provide startup instructions. Each compiler has a different version.

## trajectory.c
This file implements a trajectory generator. It provides access to a table of trajectory values, the size of which is in a #define.

## usb_bulk_structs.c
This file provides structs used by the USB bulk interface. In it you can change the VID/PID and other settings related to USB.

## structs.c
This file provides structs for communication between the host computer and the embedded Stellaris board.
