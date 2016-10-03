################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/evaluator.c \
../src/global.c \
../src/printer.c \
../src/reader.c \
../src/ybscheme.c 

OBJS += \
./src/evaluator.o \
./src/global.o \
./src/printer.o \
./src/reader.o \
./src/ybscheme.o 

C_DEPS += \
./src/evaluator.d \
./src/global.d \
./src/printer.d \
./src/reader.d \
./src/ybscheme.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c99 -O0 -g3 -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


