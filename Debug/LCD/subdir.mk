################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LCD/lcd.c 

OBJS += \
./LCD/lcd.o 

C_DEPS += \
./LCD/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
LCD/lcd.o: ../LCD/lcd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F303xC -DDEBUG -c -I../Inc -I../LCD -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Keypad -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"LCD/lcd.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

