################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../rcsLib/udpAction/udpAction.cpp 

OBJS += \
./rcsLib/udpAction/udpAction.o 

CPP_DEPS += \
./rcsLib/udpAction/udpAction.d 


# Each subdirectory must supply rules for building sources it contributes
rcsLib/udpAction/%.o: ../rcsLib/udpAction/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/tuborg/workspace/libs" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


