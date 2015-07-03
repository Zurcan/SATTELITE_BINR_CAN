################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/can.c \
../Src/gpio.c \
../Src/main.c \
../Src/stm32f2xx_hal_msp.c \
../Src/stm32f2xx_it.c \
../Src/usart.c 

OBJS += \
./Src/can.o \
./Src/gpio.o \
./Src/main.o \
./Src/stm32f2xx_hal_msp.o \
./Src/stm32f2xx_it.o \
./Src/usart.o 

C_DEPS += \
./Src/can.d \
./Src/gpio.d \
./Src/main.d \
./Src/stm32f2xx_hal_msp.d \
./Src/stm32f2xx_it.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DSTM32F205xx -I"C:\workspaceluna\SATTELITE_BINR_CAN\Inc" -I"C:\workspaceluna\SATTELITE_BINR_CAN\Drivers\CMSIS\Include" -I"C:\workspaceluna\SATTELITE_BINR_CAN\Drivers\STM32F2xx_HAL_Driver\Inc" -I"C:\workspaceluna\SATTELITE_BINR_CAN\Drivers\CMSIS\Device\ST\STM32F2xx\Include" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


