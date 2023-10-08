/**
* platform_stm32_c
*
* This file contains a blank platform code, you can use this as
* a start point to create your own chipset lib.
* So, what you think about forking this project and sharing
* your platform code? :)
*
* Don't forget! Remove all PLATFORM_ASSERT(0) when defining your code
*
*  Created on: Jun, 7 of 2023
*      Author: Pablo Jean
*/

#include "platform/platform.h"


void _platform_gpio_write(gpio_t Gpio, uint8_t Val){
	/**
	 * Here, you will define the function that writes the digital
	 * value on the desired gpio.
	 */
	PLATFORM_ASSERT(0);
}

uint8_t _platform_gpio_read(gpio_t Gpio){
	/**
	 * This routine will read the gpio value
	 * (at the moment, the lib do not use this
	 * function
	 */
	PLATFORM_ASSERT(0);
}

void _platform_pwm_control(pwm_t PwmHandle, uint8_t duty){
	/**
	 * This function must sets a PWM duty cycle value.
	 * The duty value starts from 0 to 100, where 0
	 * is the minimum power (turned off) and 100
	 * is the maximum value (always on)
	 */
	PLATFORM_ASSERT(0);
}

void _platform_delay_ms(uint32_t ms){
	/**
	 * A function that holds the program in
	 * milliseconds, a regular delay.
	 */
	PLATFORM_ASSERT(0);
}

void _platform_delay_us(uint32_t us){
	/**
	 * A function that holds the program in
	 * microseconds, some chipsets delivery to
	 * you a micros routine. Ohter, like STM32
	 * do not. Or, you implement a simple loop,
	 * or use a timer to do this job.
	 */
	PLATFORM_ASSERT(0);
}
