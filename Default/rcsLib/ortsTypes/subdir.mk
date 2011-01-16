################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../rcsLib/ortsTypes/ortsTypes.cpp 

OBJS += \
./rcsLib/ortsTypes/ortsTypes.o 

CPP_DEPS += \
./rcsLib/ortsTypes/ortsTypes.d 


# Each subdirectory must supply rules for building sources it contributes
rcsLib/ortsTypes/%.o: ../rcsLib/ortsTypes/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


