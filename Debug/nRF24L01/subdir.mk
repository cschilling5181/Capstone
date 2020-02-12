################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nRF24L01/MY_NRF24.c 

OBJS += \
./nRF24L01/MY_NRF24.o 

C_DEPS += \
./nRF24L01/MY_NRF24.d 


# Each subdirectory must supply rules for building sources it contributes
nRF24L01/MY_NRF24.o: ../nRF24L01/MY_NRF24.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F303xC -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Keypad -I../LCD -I../nRF24L01 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"nRF24L01/MY_NRF24.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

