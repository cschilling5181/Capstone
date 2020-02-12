################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include nRF24L01/subdir.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include LCD/subdir.mk
-include Keypad/subdir.mk
-include Drivers/STM32F3xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
STM32F303CapstoneProfessor.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32F303CapstoneProfessor.list \


# All Target
all: STM32F303CapstoneProfessor.elf secondary-outputs

# Tool invocations
STM32F303CapstoneProfessor.elf: $(OBJS) $(USER_OBJS) E:\Libraries\Documents\Conestoga\Fall2019\Capstone\STM32F303CapstoneProfessor\STM32F303VCTX_FLASH.ld
	arm-none-eabi-gcc -o "STM32F303CapstoneProfessor.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"E:\Libraries\Documents\Conestoga\Fall2019\Capstone\STM32F303CapstoneProfessor\STM32F303VCTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="STM32F303CapstoneProfessor.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32F303CapstoneProfessor.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32F303CapstoneProfessor.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
