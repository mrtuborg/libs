################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../rcsLib/udp_port/udp_port.cpp 

OBJS += \
./rcsLib/udp_port/udp_port.o 

CPP_DEPS += \
./rcsLib/udp_port/udp_port.d 


# Each subdirectory must supply rules for building sources it contributes
rcsLib/udp_port/%.o: ../rcsLib/udp_port/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


