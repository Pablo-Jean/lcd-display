/**
* platform_stm32_c
*
* This file contains the platform code for stm32f4 using HAL library.
* If your Stm32 chipset was from another family, but you're using HAL,
* just modify the include file to your device.
*
*  Created on: Jun, 6 of 2023
*      Author: Pablo Jean
*/

#include "platform/platform.h"

#include <stm32f1xx.h>

void _platform_gpio_write(gpio_t Gpio, uint8_t Val){
	GPIO_TypeDef *gpio = (GPIO_TypeDef*)Gpio.GPIO;
	uint32_t pin = Gpio.pin;

	HAL_GPIO_WritePin(gpio, pin, (GPIO_PinState)Val);
}

uint8_t _platform_gpio_read(gpio_t Gpio){
	GPIO_TypeDef *gpio = (GPIO_TypeDef*)Gpio.GPIO;
	uint32_t pin = Gpio.pin;

	return (uint8_t)HAL_GPIO_ReadPin(gpio, pin);
}

void _platform_pwm_control(pwm_t PwmHandle, uint8_t duty){
	return;
}

void _platform_delay_ms(uint32_t ms){
	HAL_Delay(ms);
}

void _platform_delay_us(uint32_t us){
	uint32_t sysclk = HAL_RCC_GetSysClockFreq();
	uint32_t ticks = ((sysclk)/(4E6))*us;

	while (ticks > 0){
		ticks--;
	}
}
