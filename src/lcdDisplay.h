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

#include "platform/platform.h"

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

typedef struct{
	gpio_t gpios[LCD_PIN_QTD];
	uint32_t columns;
	uint32_t rows;
	lcd_interface_mode_e interface;
	lcd_font_type_e font;
	//backlight control, pick one, or no one
	gpio_t backlightGpio;
	pwm_t backlightPwm;
	// please, do not customize this values
	uint8_t _cursor;
	uint8_t _column;
	uint8_t _row;
}lcd_t;

/*
 * Function Prototypes
 */

void lcd_init (lcd_t *lcd);   // initialize lcd

void lcd_blink_off(lcd_t *lcd);

void lcd_blink_on(lcd_t *lcd);

void lcd_cursor_off(lcd_t *lcd);

void lcd_cursor_on(lcd_t *lcd);

void lcd_display_off(lcd_t *lcd);

void lcd_display_on(lcd_t *lcd);

void lcd_cmd (lcd_t *lcd, lcd_cmd_e cmd);  // send command to the lcd

void lcd_data (lcd_t *lcd, uint8_t data);

void lcd_put_pos(lcd_t *lcd, uint8_t row, uint8_t col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_send_char (lcd_t *lcd, char data);  // send data to the lcd

void lcd_send_string (lcd_t *lcd, char *str);  // send string to the lcd

void lcd_send_char_pos(lcd_t *lcd, char data, uint8_t row, uint8_t col);

void lcd_send_string_pos (lcd_t *lcd, char *str, uint8_t row, uint8_t col);

void lcd_clear_row (lcd_t *lcd, uint8_t row);

void lcd_clear_all (lcd_t *lcd);

void lcd_create_custom_char (lcd_t *lcd, lcd_custom_char_e custom, uint8_t *bitmap);

void lcd_backlight_set (lcd_t *lcd, uint8_t state);

void lcd_backlight_set_bright (lcd_t *lcd, uint8_t level);

#endif /* INC_LCD1602_H_ */
