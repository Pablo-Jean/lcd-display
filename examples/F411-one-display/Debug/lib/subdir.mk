################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/mnt/A68A1E388A1E0589/GoogleDrive/Pablo\ Arquivos/Projetos/Library/Proprietarias/lcd/src/lcdDisplay.c 

OBJS += \
./lib/lcdDisplay.o 

C_DEPS += \
./lib/lcdDisplay.d 


# Each subdirectory must supply rules for building sources it contributes
lib/lcdDisplay.o: /mnt/A68A1E388A1E0589/GoogleDrive/Pablo\ Arquivos/Projetos/Library/Proprietarias/lcd/src/lcdDisplay.c lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../../../src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"lib/lcdDisplay.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lib

clean-lib:
	-$(RM) ./lib/lcdDisplay.cyclo ./lib/lcdDisplay.d ./lib/lcdDisplay.o ./lib/lcdDisplay.su

.PHONY: clean-lib

