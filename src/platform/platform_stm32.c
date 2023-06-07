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

#include "platform.h"

#include <stm32f4xx.h>

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
	TIM_HandleTypeDef *htim = (TIM_HandleTypeDef*)PwmHandle.Peripheral;
	uint32_t channel = PwmHandle.Channel;

	if (duty == 0){
		HAL_TIM_PWM_Stop(htim, channel);
	}
	else if (duty > 100){
		duty = 100;
	}
	if (duty != 0){
		uint32_t arr = __HAL_TIM_GET_AUTORELOAD(htim);
		uint32_t cmp = arr*duty/100;

		__HAL_TIM_SET_COMPARE(htim, channel, cmp);
		HAL_TIM_PWM_Start(htim, channel);
	}
}

void _platform_delay_ms(uint32_t ms){
	HAL_Delay(ms);
}

void _platform_delay_us(uint32_t us){
	uint32_t sysclk = HAL_RCC_GetSysClockFreq();
	uint32_t ticks = ((sysclk)/(2E6))*us;

	while (ticks > 0){
		ticks--;
	}
}
