################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Keypad/keypad.c 

OBJS += \
./Keypad/keypad.o 

C_DEPS += \
./Keypad/keypad.d 


# Each subdirectory must supply rules for building sources it contributes
Keypad/keypad.o: ../Keypad/keypad.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F303xC -DDEBUG -c -I../Inc -I../LCD -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Keypad -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Keypad/keypad.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

