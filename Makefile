#******************************************************************************
# MakeFile originally from StellarisWare Examples.
# Parts Copyright 2012 Texas Instruments
# Modified by Paul Bartell
#******************************************************************************

#
# Defines the part type that this project uses.
#
PART=LM4F120H5QR

#
# Set the processor variant.
#
VARIANT=cm4f

#
# The base directory for StellarisWare.
#
ROOT=/Users/paulb/stellaris/stellaris

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH=
VPATH+=${ROOT}/utils

#
# Where to find header files that do not live in the source directory.
#
IPATH=${ROOT}/

#
# The default rule, which causes the USB device bulk example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/tissue_tester.axf

#
# The Rule for flashing the endpoint board
#

flash:
	@lm4flash ${COMPILER}/tissue_tester.bin

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the USB device bulk example.
#
${COMPILER}/tissue_tester.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/tissue_tester.axf: ${COMPILER}/main.o
${COMPILER}/tissue_tester.axf: ${COMPILER}/uartstdio.o
#${COMPILER}/tissue_tester.axf: ${COMPILER}/usb_bulk_structs.o
#${COMPILER}/tissue_tester.axf: ${COMPILER}/usb_dev_bulk.o
${COMPILER}/tissue_tester.axf: ${COMPILER}/ustdlib.o
${COMPILER}/tissue_tester.axf: ${ROOT}/usblib/${COMPILER}-cm4f/libusb-cm4f.a
${COMPILER}/tissue_tester.axf: ${ROOT}/driverlib/${COMPILER}-cm4f/libdriver-cm4f.a
#${COMPILER}/tissue_tester.axf: usb_dev_bulk.ld
SCATTERgcc_tissue_tester=tissue_tester.ld
ENTRY_tissue_tester=ResetISR
CFLAGSgcc=-DTARGET_IS_BLIZZARD_RA1 -DUART_BUFFERED

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
