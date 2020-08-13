################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
GPS_Get_location.lss \

FLASH_IMAGE += \
GPS_Get_location.hex \

EEPROM_IMAGE += \
GPS_Get_location.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: GPS_Get_location.elf secondary-outputs

# Tool invocations
GPS_Get_location.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,GPS_Get_location.map -mmcu=atmega32 -o "GPS_Get_location.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

GPS_Get_location.lss: GPS_Get_location.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S GPS_Get_location.elf  >"GPS_Get_location.lss"
	@echo 'Finished building: $@'
	@echo ' '

GPS_Get_location.hex: GPS_Get_location.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex GPS_Get_location.elf  "GPS_Get_location.hex"
	@echo 'Finished building: $@'
	@echo ' '

GPS_Get_location.eep: GPS_Get_location.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex GPS_Get_location.elf  "GPS_Get_location.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: GPS_Get_location.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 GPS_Get_location.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) GPS_Get_location.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
