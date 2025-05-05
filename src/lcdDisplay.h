/*
 * lcdDisplay.h
 *
 *  Created on: Jun, 6 of 2023
 *      Author: Pablo Jean
 */

#ifndef INC_LCDDISPLAY_H_
#define INC_LCDDISPLAY_H_


#include <stdint.h>
#include <stddef.h>
#include <assert.h>

/**
 * Macros
 */

#define DISPLAY_CONTROL_ON		(1 << 2)
#define DISPLAY_CURSOR_ON		(1 << 1)
#define DISPLAY_BLINK_ON		(1)
#define DISPLAY_CONTROL_OFF		(0)
#define DISPLAY_CURSOR_OFF		(0)
#define DISPLAY_BLINK_OFF		(0)

#define DISPLAY_DL_4BIT			0
#define DISPLAY_DL_8BIT			(1<<4)
#define DISPLAY_N_1LINE			0
#define DISPLAY_N_2LINE			(1<<3)
#define DISPLAY_FONT_5x8		0
#define DISPLAY_FONT_5x10		(1<<2)

#define LCD_BACKLIGHT_ON		1
#define LCD_BACKLIGHT_OFF		0

#define LCD_RS_ON				1
#define LCD_RS_OFF				0

#define LCD_CHAR_ROWS			8

/*
 * Enumerates
 */
typedef enum{
	LCD_CMD_CLEAR = 0x01,
	LCD_CMD_RETURN_HOME = 0x02,
	LCD_CMD_DEC_CURSOR = 0x04,
	LCD_CMD_INC_CURSOR = 0x06,
	LCD_CMD_SHIFT_RIGHT = 0x05,
	LCD_CMD_SHIFT_LEFT = 0x07,
	LCD_CMD_DISP_CONTROL = 0x08,
	LCD_CMD_SHIFT_CUR_LEFT = 0x10,
	LCD_CMD_SHIFT_CUR_RIGHT = 0x14,
	LCD_CMD_SHIFT_DISP_LEFT = 0x18,
	LCD_CMD_SHIFT_DISP_RIGHT = 0x1C,
	LCD_CMD_FUNCTION_SET = 0x20,
	LCD_CMD_SET_CGRAM = 0x40,
}lcd_cmd_e;

typedef enum{
	LCD_CUSTOM_1,
	LCD_CUSTOM_2,
	LCD_CUSTOM_3,
	LCD_CUSTOM_4,
	LCD_CUSTOM_5,
	LCD_CUSTOM_6,
	LCD_CUSTOM_7,
	LCD_CUSTOM_8
}lcd_custom_char_e;

typedef enum{
	LCD_INTERFACE_4BIT,
	LCD_INTERFACE_8BIT
}lcd_interface_mode_e;

typedef enum{
	LCD_FONT_5X8,
	LCD_FONT_5X10
}lcd_font_type_e;

typedef enum{
	LCD_RS,
	LCD_RW,
	LCD_E,
	LCD_D0,
	LCD_D1,
	LCD_D2,
	LCD_D3,
	LCD_D4,
	LCD_D5,
	LCD_D6,
	LCD_D7,

	LCD_PIN_QTD
}lcd_pin_e;

/* Auxiliary Functions */

typedef void (*lcd_gpio_fxn_t)(lcd_pin_e pin, uint8_t state);
typedef void (*lcd_backlight_fxn_t)(uint8_t value);
typedef void (*lcd_delay_us_fxn_t)(uint32_t us);

/* Do not edit this struct, configure using the lcd_params_t*/
typedef struct{
	/* Function to control the GPIOs of the microcontroller */
	lcd_gpio_fxn_t GpioFxn;
	/* Function to set the backlight, from 0 to 100, you can use a simple gpio turn on/off
	 * or use a PWM to set a Brightness level
	 */
	lcd_backlight_fxn_t BacklightFxn;
	/* Delay us wrapper function*/
	lcd_delay_us_fxn_t DelayUsFxn;
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
	// !! please, do not customize this values !!
	// todo: implement this varaibles inside a struct
	uint8_t _cursor;
	uint8_t _column;
	uint8_t _row;
	uint8_t _initialized;
	// finished private variables
}lcd_t;

typedef struct{
	/* (Mandatory) Function to control the GPIOs of the microcontroller */
	lcd_gpio_fxn_t GpioFxn;

	/* (Optional) Function to set the backlight, from 0 to 100, you can use a simple gpio turn on/off
	 * or use a PWM to set a Brightness level
	 */
	lcd_backlight_fxn_t BacklightFxn;

	/* (Mandatory) Delay us wrapper function, must be implemented for correctly timing on startup */
	lcd_delay_us_fxn_t DelayUsFxn;

	/* (Mandatory) How amount Columns you Lcd have, if you display is a 
	 * 20x4, so, your columns will be 20
	 */
	uint32_t columns;

	/* (Mandatory) How amount Rows you Lcd have, if you display is a 
	 * 20x4, so, your rows will be 4
	 */
	uint32_t rows;

	/* (Mandatory) The program must know how you wired the Data pins of your LCD.
	 * LCD_INTERFACE_4BIT: if you connected D4~D7 pins
	 * LCD_INTERFACE_8BIT: if you connected D0~D7 pins
	 */
	lcd_interface_mode_e interface;

	/* (Mandatory) Tell the font sizing of the display.
	 * LCD_FONT_5X8: Most common
	 * LCD_FONT_5X10: A bigger version of the lcd display.
	 */
	lcd_font_type_e font;
}lcd_params_t;

/*
 * Function Prototypes
 */

/**
 * @brief Initialize the display.
 * 
 * @param lcd : pointer to the Lcd Handler
 * @param params: pointer to the Lcd Initialization parameters
 */
void lcd_init (lcd_t *lcd, lcd_params_t *params);  

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

#endif /* INC_LCD1602_H_ */
