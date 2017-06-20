################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/ADCChannel.cpp \
../Source/ADCMCP3008.cpp \
../Source/ControllData.cpp \
../Source/GpioExpanderMCP23017.cpp \
../Source/I2CAccess.cpp \
../Source/MainThread.cpp \
../Source/SPIAccess.cpp \
../Source/ServoController.cpp \
../Source/ServoMotor.cpp \
../Source/main.cpp \
../Source/rrdarchive.cpp 

OBJS += \
./Source/ADCChannel.o \
./Source/ADCMCP3008.o \
./Source/ControllData.o \
./Source/GpioExpanderMCP23017.o \
./Source/I2CAccess.o \
./Source/MainThread.o \
./Source/SPIAccess.o \
./Source/ServoController.o \
./Source/ServoMotor.o \
./Source/main.o \
./Source/rrdarchive.o 

CPP_DEPS += \
./Source/ADCChannel.d \
./Source/ADCMCP3008.d \
./Source/ControllData.d \
./Source/GpioExpanderMCP23017.d \
./Source/I2CAccess.d \
./Source/MainThread.d \
./Source/SPIAccess.d \
./Source/ServoController.d \
./Source/ServoMotor.d \
./Source/main.d \
./Source/rrdarchive.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"C:\Users\Philipp\Documents\Programme\Raspberrypi\Rauchofen\Include" -I"C:\Users\Philipp\Documents\Programme\Raspberrypi\W1_Temp\Include" -I"C:\Users\Philipp\Documents\Programme\Raspberrypi\BaseThread\Include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


