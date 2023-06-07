/*
 * platform.h
 *
 * This file contains all header files to make the bridge between
 * the lcd display logic and the chipset, to allow the library
 * works with any microcontroller withtout the needing any change
 * on the src code.
 *
 * This file can be associated with only one platform_xxx.c file
 * Do not include on source more than one .c file, or errors must
 * occour.
 *
 *  Created on: Jun, 6 of 2023
 *      Author: Pablo Jean
 */

#include <stdint.h>
#include <stddef.h>

/**
 * Macros
 */

#define PLATFORM_ASSERT(x)		if(x == 0) {while(1);}

/**
 * Typedefs
 */

typedef struct{
	uint32_t GPIO;
	uint32_t pin;
}gpio_t;

typedef struct{
	uint32_t Peripheral;
	uint32_t Channel;
}pwm_t;

/**
 * Publics
 */

void _platform_gpio_write(gpio_t Gpio, uint8_t Val);

uint8_t _platform_gpio_read(gpio_t Gpio);

void _platform_pwm_control(pwm_t PwmHandle, uint8_t duty);

void _platform_delay_ms(uint32_t ms);

void _platform_delay_us(uint32_t us);
