################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Quadcontrol.cpp \
../src/Quadcopter.cpp \
../src/serialib.cpp 

OBJS += \
./src/Quadcontrol.o \
./src/Quadcopter.o \
./src/serialib.o 

CPP_DEPS += \
./src/Quadcontrol.d \
./src/Quadcopter.d \
./src/serialib.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


