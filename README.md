<center>
<img src="imgs/logo-education.png"  width="300">

Created for Ouroboros Embedded Education.
</center>

## Introduction

This is a simple library to work with the LCD Alphanumeric Display, encountered in many sizes and colors.

An applicable LCD Display can be seen below:

<center><img src="imgs/lcd-blue.png"  width="200"></center>

This lib aiming to provide this advantages:

- Allow use any amount of displays you want;
- Easy port to any chipsets, without any direct link with the microcontroller;
- Easy to configure;
- For noobies and professionais.

## Files

### lcdDisplay.c / lcdDisplay.h

Contains the Lcd logic functions and the Lcd Handler.

### Platform Folder

In this folder we have the `platform.h` file with contains the chipset functions must be implemented by the user.

The `platform_blank.c` contains only the template to start the writing. I recommend you to copy this file and paste on your application folder. Please, do not forget to remove this file from your build configuration.

## Structure

The `lcd_t` handler will be explained below:

```C
typedef struct{
	/* The array containing the GPIO and Pins where the Lcd is connected */
	gpio_t gpios[LCD_PIN_QTD];
	/* How amount Columns you Lcd have, if you display is a 
	 * 20x4, so, your columns will be 20
	 */
	uint32_t columns;
	/* How amount Rows you Lcd have, if you display is a 
	 * 20x4, so, your rows will be 4
	 */
	uint32_t rows;
	/* The program must know how you wired the Data pins of your LCD.
	 * LCD_INTERFACE_4BIT: if you connected D4~D7 pins
	 * LCD_INTERFACE_8BIT: if you connected D0~D7 pins
	 */
	lcd_interface_mode_e interface;
	/* Tell the font sizing of the display.
	 * LCD_FONT_5X8: Most common
	 * LCD_FONT_5X10: A bigger version of the lcd display.
	 */
	lcd_font_type_e font;
	//backlight control, pick one, or no one
	/* Only Set one of this parameters if you have the backlight control */
	/* If you controlling a On/Off of the LCD, inform the GPIO and Pin
	 * For the moment, the HIGH level is to turn on the LCD (NPN Transistor)
	 */
	gpio_t backlightGpio;
	/* Provide the Timer Peripheral Handler and the Channel of
	 * the PWM channel to set the Lcd brightness level over the lib.
	 * 100% indicates full brightness, and 0% will turnoff.
	 */
	pwm_t backlightPwm;
	// !! please, do not customize this values !!
	uint8_t _cursor;
	uint8_t _column;
	uint8_t _row;
}lcd_t;
```

And, to the handler knows what GPIOs to drive, the  `gpio_t` is the type of the `gpios` parameter on the `lcd_t` handler. So, the developer must inform all connected GPIOs.

```C
typedef struct{
	/* The GPIO handler of your MCU, some uc do not have
	 * the GPIO handler, only the Pin (nRF52810, CC26x2R1, for example)
	 */
	uint32_t GPIO;
	/* The Pin number parameter. Some MCUs has only this parameters
	 * to identify your GPIOs*/
	uint32_t pin;
	/* Example of providing this parameters:
	 * ::for STM32::
	 * gpios.GPIO = (uint32_t)GPIOA
	 * gpios.pin = GPIO_PIN3
	 */
}gpio_t;
```

Note: I will talk about PWM later.

## Functions

```C
/**
 * @brief Initialize the display.
 * 
 * @param lcd : pointer to the Lcd Handler
 */
void lcd_init (lcd_t *lcd);  

/**
 * @brief Disables the cursor blink.
 * 
 * @param lcd : pointer to the Lcd Handler
 */
void lcd_blink_off(lcd_t *lcd);

/**
 * @brief Enables the cursor blink
 * 
 * @param lcd : pointer to the Lcd Handler
 */
void lcd_blink_on(lcd_t *lcd);

/**
 * @brief Disables the underline cursor
 * 
 * @param lcd : pointer to the Lcd Handler
 */
void lcd_cursor_off(lcd_t *lcd);

/**
 * @brief Enables the underline cursor
 * 
 * @param lcd : pointer to the Lcd Handler
 */
void lcd_cursor_on(lcd_t *lcd);

/**
 * @brief Turnoff the display without losing data
 * 
 * @param lcd : pointer to the Lcd Handler
 */
void lcd_display_off(lcd_t *lcd);

/**
 * @brief Turn on back the display
 * 
 * @param lcd : pointer to the Lcd Handler
 */
void lcd_display_on(lcd_t *lcd);

/**
 * @brief Sends a Command to the display;
 * 
 * @param lcd : pointer to the Lcd Handler
 * @param cmd : Command to issue to the Lcd, see lcd_cmd_e enum.
 */
void lcd_cmd (lcd_t *lcd, lcd_cmd_e cmd);  // send command to the lcd

/**
 * @brief Issue a raw byte data to the Lcd
 * 
 * @param lcd  : pointer to the Lcd Handler
 * @param data : the data byte
 */
void lcd_data (lcd_t *lcd, uint8_t data);

/**
 * @brief Change the cursor position on the Lcd pannel.
 * 
 * @param lcd : pointer to the Lcd Handler
 * @param row : desired row. The command will not be sent if row is greater than lcd->rows.
 * @param col : desired column. The command will not be sent if row is greater than lcd->columns.
 */
void lcd_put_pos(lcd_t *lcd, uint8_t row, uint8_t col);  

/**
 * @brief Send a char on the actual cursor position. Char can be a custom char, see the lcd_custom_char_e.
 * 
 * @param lcd : pointer to the Lcd Handler
 * @param data : the desired char
 */
void lcd_send_char (lcd_t *lcd, char data);  

/**
 * @brief Send a string, that starts on the actual cursor position. The library will 
 * automatically stops when reach the end of the display.
 * 
 * @param lcd : pointer to the Lcd Handler
 * @param str : array of characters containing the string
 */
void lcd_send_string (lcd_t *lcd, char *str);  // send string to the lcd

/**
 * @brief Send a char on the desired position
 * 
 * @param lcd : pointer to the Lcd Handler
 * @param data : the desired char
 * @param row : desired row. The command will not be sent if row is greater than lcd->rows.
 * @param col : desired column. The command will not be sent if row is greater than lcd->columns.
 */
void lcd_send_char_pos(lcd_t *lcd, char data, uint8_t row, uint8_t col);

/**
 * @brief Send a string, that starts on the provided row and column. The library will 
 * automatically stops when reach the end of the display.
 * 
 * @param lcd : pointer to the Lcd Handler
 * @param str : array of characters containing the string
 * @param row : desired row. The command will not be sent if row is greater than lcd->rows.
 * @param col : desired column. The command will not be sent if row is greater than lcd->columns.
 */
void lcd_send_string_pos (lcd_t *lcd, char *str, uint8_t row, uint8_t col);

/**
 * @brief Fills with blank only the desired row.
 * 
 * @param lcd : pointer to the Lcd Handler
 * @param row : the row to be cleared.
 */
void lcd_clear_row (lcd_t *lcd, uint8_t row);

/**
 * @brief Clears the entire display
 * 
 * @param lcd : pointer to the Lcd Handler
 */
void lcd_clear_all (lcd_t *lcd);

/**
 * @brief Creates a custom character
 * 
 * @param lcd : pointer to the Lcd Handler
 * @param custom : The custom position starting from LCD_CUSTOM_1 to LCD_CUSTOM_8
 * @param bitmap : The bitmap array
 */
void lcd_create_custom_char (lcd_t *lcd, lcd_custom_char_e custom, uint8_t *bitmap);

/**
 * @brief Sets the display backlight to on or off.
 * 
 * @param lcd : pointer to the Lcd Handler
 * @param state : LCD_BACKLIGHT_ON to turnon display or LCD_BACKLIGHT_OFF to turnoff (really?)
 */
void lcd_backlight_set (lcd_t *lcd, uint8_t state);

/**
 * @brief Sets the display brightness based on a percentage. To use this, the developer
 * must implements an PWM.
 * 
 * @param lcd : pointer to the Lcd Handler
 * @param level : You can set this value from 0 to 100, where 0 is the minumum brightness, and
 * 100, the maximum.
 */
void lcd_backlight_set_bright (lcd_t *lcd, uint8_t level);
```

## Quick Start Steps

See the example.

## Examples

We have one provided for the moment.
