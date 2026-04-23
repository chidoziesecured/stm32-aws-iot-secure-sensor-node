################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include IOT_SDK/mqtt_helper/subdir.mk
-include IOT_SDK/Thirdparty/aws-iot-device-sdk-libraries/standard/coreJSON/source/subdir.mk
-include IOT_SDK/Thirdparty/aws-iot-device-sdk-libraries/aws/aws-iot-core-mqtt-file-streams/source/subdir.mk
-include IOT_SDK/Thirdparty/aws-iot-device-sdk-libraries/aws/Jobs-for-AWS-IoT-embedded-sdk/source/otaJobParser/subdir.mk
-include IOT_SDK/Thirdparty/aws-iot-device-sdk-libraries/aws/Jobs-for-AWS-IoT-embedded-sdk/source/subdir.mk
-include IOT_SDK/Thirdparty/aws-iot-device-sdk-libraries/3rdparty/tinycbor/src/subdir.mk
-include IOT_SDK/Thirdparty/FreeRTOS-Kernel/portable/MemMang/subdir.mk
-include IOT_SDK/Thirdparty/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/subdir.mk
-include IOT_SDK/Thirdparty/FreeRTOS-Kernel/subdir.mk
-include IOT_SDK/BSP/utils/subdir.mk
-include IOT_SDK/BSP/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
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

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := aws_MQQT_Subscribe_Publish
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
aws_MQQT_Subscribe_Publish.elf \

MAP_FILES += \
aws_MQQT_Subscribe_Publish.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
aws_MQQT_Subscribe_Publish.list \


# All Target
all: main-build

# Main-build Target
main-build: aws_MQQT_Subscribe_Publish.elf secondary-outputs

# Tool invocations
aws_MQQT_Subscribe_Publish.elf aws_MQQT_Subscribe_Publish.map: $(OBJS) $(USER_OBJS) /Users/chidozie/Desktop/AWS_IoT/Project_1/aws_MQQT_Subscribe_Publish/STM32F446RETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "aws_MQQT_Subscribe_Publish.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/Users/chidozie/Desktop/AWS_IoT/Project_1/aws_MQQT_Subscribe_Publish/STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="aws_MQQT_Subscribe_Publish.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

aws_MQQT_Subscribe_Publish.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "aws_MQQT_Subscribe_Publish.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) aws_MQQT_Subscribe_Publish.elf aws_MQQT_Subscribe_Publish.list aws_MQQT_Subscribe_Publish.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
