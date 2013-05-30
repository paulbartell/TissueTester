################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ADCSetup.obj: ../ADCSetup.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="ADCSetup.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PID.obj: ../PID.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="PID.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PWMSetup.obj: ../PWMSetup.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="PWMSetup.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

accel.obj: ../accel.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="accel.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

app_commands.obj: ../app_commands.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="app_commands.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

cmdline.obj: ../cmdline.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="cmdline.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

newpins.obj: ../newpins.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="newpins.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

sine.obj: ../sine.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="sine.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

trajectory.obj: ../trajectory.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="trajectory.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uartstdio.obj: C:/StellarisWare/utils/uartstdio.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="uartstdio.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ustdlib.obj: ../ustdlib.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_4.9.7/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_4.9.7/include" --include_path="C:/StellarisWare/utils" --include_path="C:/StellarisWare" --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="ustdlib.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


