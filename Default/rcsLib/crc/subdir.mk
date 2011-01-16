################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../rcsLib/crc/crc16.cpp 

OBJS += \
./rcsLib/crc/crc16.o 

CPP_DEPS += \
./rcsLib/crc/crc16.d 


# Each subdirectory must supply rules for building sources it contributes
rcsLib/crc/%.o: ../rcsLib/crc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


