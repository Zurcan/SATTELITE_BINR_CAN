################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/system_stm32f2xx.c 

OBJS += \
./Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/system_stm32f2xx.o 

C_DEPS += \
./Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/system_stm32f2xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/%.o: ../Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DSTM32F205xx -I"C:\workspaceluna\1\Inc" -I"C:\workspaceluna\1\Drivers\CMSIS\Include" -I"C:\workspaceluna\1\Drivers\STM32F2xx_HAL_Driver\Inc" -I"C:\workspaceluna\1\Drivers\CMSIS\Device\ST\STM32F2xx\Include" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


