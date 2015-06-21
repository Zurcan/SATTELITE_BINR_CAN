################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/gcc/startup_stm32f2xx.S 

OBJS += \
./Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/gcc/startup_stm32f2xx.o 

S_UPPER_DEPS += \
./Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/gcc/startup_stm32f2xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/gcc/%.o: ../Drivers/CMSIS/Device/ST/STM32F2xx/Source/Templates/gcc/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -x assembler-with-cpp -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DSTM32F205xx -I"C:\stm32Eclipse\workspace\SATTELITE_BINR_CAN\Inc" -I"C:\stm32Eclipse\workspace\SATTELITE_BINR_CAN\Drivers\CMSIS\Include" -I"C:\stm32Eclipse\workspace\SATTELITE_BINR_CAN\Drivers\STM32F2xx_HAL_Driver\Inc" -I"C:\stm32Eclipse\workspace\SATTELITE_BINR_CAN\Drivers\CMSIS\Device\ST\STM32F2xx\Include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


