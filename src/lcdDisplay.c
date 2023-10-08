/*
 * LCD1602.c
 *
 *  Created on: 21-Jan-2020
 *      Author: Controllerstech
 */

#include "lcdDisplay.h"


/****************************************************************************************************************************************************************/

void send_to_lcd (lcd_t *lcd, char data, int rs)
{
	uint8_t _dataSend[2], steps, i;

	_platform_gpio_write(lcd->gpios[LCD_RS], rs);  // rs = 1 for data, rs=0 for command
	_platform_delay_us(5);
	if (lcd->interface == LCD_INTERFACE_4BIT){
		// will send the dara byte in two steps
		// first is sent the Upper 4 bits (higher nibble) over the interface
		// in the second step, is sent the lower nibble
		steps = 2;
		_dataSend[0] = ((data >> 4) & 0xF);
		_dataSend[1] = (data & 0xF);
		for (i=0 ; i<steps ; i++){
			/* write the data to the respective pin */
			_platform_gpio_write(lcd->gpios[LCD_D7], ((_dataSend[i]>>3)&0x01));
			_platform_gpio_write(lcd->gpios[LCD_D6], ((_dataSend[i]>>2)&0x01));
			_platform_gpio_write(lcd->gpios[LCD_D5], ((_dataSend[i]>>1)&0x01));
			_platform_gpio_write(lcd->gpios[LCD_D4], ((_dataSend[i]>>0)&0x01));

			/* Toggle EN PIN to send the data
			 */
			_platform_delay_us(25);
			_platform_gpio_write(lcd->gpios[LCD_E], 1);
			_platform_delay_us(1);
			_platform_gpio_write(lcd->gpios[LCD_E], 0);
		}
	}
	else{
		_dataSend[0] = data;
		/* write the data to the respective pin */
		_platform_gpio_write(lcd->gpios[LCD_D7], ((_dataSend[0]>>7)&0x01));
		_platform_gpio_write(lcd->gpios[LCD_D6], ((_dataSend[0]>>6)&0x01));
		_platform_gpio_write(lcd->gpios[LCD_D5], ((_dataSend[0]>>5)&0x01));
		_platform_gpio_write(lcd->gpios[LCD_D4], ((_dataSend[0]>>4)&0x01));
		_platform_gpio_write(lcd->gpios[LCD_D3], ((_dataSend[0]>>3)&0x01));
		_platform_gpio_write(lcd->gpios[LCD_D2], ((_dataSend[0]>>2)&0x01));
		_platform_gpio_write(lcd->gpios[LCD_D1], ((_dataSend[0]>>1)&0x01));
		_platform_gpio_write(lcd->gpios[LCD_D0], ((_dataSend[0]>>0)&0x01));

		/* Toggle EN PIN to send the data
		 */
		_platform_delay_us(25);
		_platform_gpio_write(lcd->gpios[LCD_E], 1);
		_platform_delay_us(1);
		_platform_gpio_write(lcd->gpios[LCD_E], 0);
	}
}

/**
 * Publics
 */

void lcd_init (lcd_t *lcd)
{
	uint8_t dispDl, dispFont;

	if (lcd->font == LCD_FONT_5X8){
		dispFont = DISPLAY_FONT_5x8;
	}
	else{
		dispFont = DISPLAY_FONT_5x10;
	}
	_platform_delay_ms(40);  	// wait for >40ms
	lcd_cmd (lcd, LCD_CMD_FUNCTION_SET | DISPLAY_DL_8BIT);
	_platform_delay_ms(5);  	// wait for >4.1ms
	lcd_cmd (lcd, LCD_CMD_FUNCTION_SET | DISPLAY_DL_8BIT);
	_platform_delay_us(100);  	// wait for >100us
	lcd_cmd (lcd, LCD_CMD_FUNCTION_SET | DISPLAY_DL_8BIT);
	_platform_delay_ms(100);	// wait for >100us
	if (lcd->interface == LCD_INTERFACE_4BIT){
		// 4 bit initialisation
		lcd_cmd (lcd, LCD_CMD_FUNCTION_SET | DISPLAY_DL_4BIT);  // 4bit mode
		dispDl = DISPLAY_DL_4BIT;
	}
	else{
		lcd_cmd (lcd, LCD_CMD_FUNCTION_SET | DISPLAY_DL_8BIT);
		dispDl = DISPLAY_DL_8BIT;
	}
	_platform_delay_ms(10);

  // dislay initialisation
	lcd->_cursor = DISPLAY_CONTROL_ON | DISPLAY_CURSOR_OFF | DISPLAY_BLINK_OFF;
	lcd_cmd (lcd, LCD_CMD_FUNCTION_SET | dispDl | DISPLAY_N_2LINE | dispFont); // Function set --> DL=0 or 1, N = 1 (2 line display) F = 0 (5x8 characters)
	_platform_delay_ms(5);
	lcd_cmd (lcd, LCD_CMD_DISP_CONTROL); //Display on/off control --> D=0,C=0, B=0  ---> display off
	_platform_delay_ms(5);
	lcd_cmd (lcd, LCD_CMD_CLEAR);  // clear display
	_platform_delay_ms(5);
	lcd_cmd (lcd, LCD_CMD_INC_CURSOR); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	_platform_delay_ms(5);
	lcd_cmd (lcd, LCD_CMD_DISP_CONTROL | lcd->_cursor); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)

	lcd_cmd(lcd, LCD_CMD_RETURN_HOME);
	lcd->_column = 0;
	lcd->_row = 0;
}

void lcd_blink_off(lcd_t *lcd){
	lcd->_cursor &= ~(DISPLAY_BLINK_ON);
	lcd_cmd(lcd, LCD_CMD_DISP_CONTROL | lcd->_cursor);
}

void lcd_blink_on(lcd_t *lcd){
	lcd->_cursor |= (DISPLAY_BLINK_ON);
	lcd_cmd(lcd, LCD_CMD_DISP_CONTROL | lcd->_cursor);
}

void lcd_cursor_off(lcd_t *lcd){
	lcd->_cursor &= ~(DISPLAY_CURSOR_ON);
	lcd_cmd(lcd, LCD_CMD_DISP_CONTROL | lcd->_cursor);
}

void lcd_cursor_on(lcd_t *lcd){
	lcd->_cursor |= (DISPLAY_CURSOR_ON);
	lcd_cmd(lcd, LCD_CMD_DISP_CONTROL | lcd->_cursor);
}

void lcd_display_off(lcd_t *lcd){
	lcd->_cursor &= ~(DISPLAY_CONTROL_ON);
	lcd_cmd(lcd, LCD_CMD_DISP_CONTROL | lcd->_cursor);
}

void lcd_display_on(lcd_t *lcd){
	lcd->_cursor |= (DISPLAY_CONTROL_ON);
	lcd_cmd(lcd, LCD_CMD_DISP_CONTROL | lcd->_cursor);
}

void lcd_cmd (lcd_t *lcd, lcd_cmd_e cmd)
{
	send_to_lcd(lcd, (uint8_t)cmd, LCD_RS_OFF);
}

void lcd_data (lcd_t *lcd, uint8_t data){
	send_to_lcd(lcd, data, LCD_RS_ON);
}

void lcd_put_pos(lcd_t *lcd, uint8_t row, uint8_t col)
{
	if (row >= lcd->rows || col >= lcd->columns){
		return;
	}
    lcd->_column = col;
    lcd->_row = row;
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
        case 2:
        	col |= 0x94;
        	break;
        case 3:
        	col |= 0xD4;
        	break;
    }
    lcd_cmd (lcd, col);
}

void lcd_send_char (lcd_t *lcd, char data){
	if (lcd->_column >= lcd->columns || lcd->_row >= lcd->rows){
		return;
	}
	lcd_data(lcd, (uint8_t)data);
	lcd->_column++;
}

void lcd_send_string (lcd_t *lcd, char *str)
{
	while (*str)lcd_send_char (lcd, (*str++));
}

void lcd_send_char_pos(lcd_t *lcd, char data, uint8_t row, uint8_t col){
	lcd_put_pos(lcd, row, col);
	lcd_send_char(lcd, data);
}

void lcd_send_string_pos (lcd_t *lcd, char *str, uint8_t row, uint8_t col){
	lcd_put_pos(lcd, row, col);
	lcd_send_string(lcd, str);
}

void lcd_clear_row (lcd_t *lcd, uint8_t row){
	uint8_t i;

	lcd_put_pos(lcd, row, 0);
	for (i=0 ; i<lcd->columns ; i++){
		lcd_send_char(lcd, ' ');
	}
}

void lcd_clear_all (lcd_t *lcd){
	lcd_cmd(lcd, LCD_CMD_CLEAR);
	_platform_delay_ms(2);
}

void lcd_create_custom_char (lcd_t *lcd, lcd_custom_char_e custom, uint8_t *bitmap){
	uint8_t i, rows, offset;

	if (lcd->font == LCD_FONT_5X8){
		rows = 8;
	}
	else{
		rows = 10;
	}
	offset = custom*0x8;
	lcd_cmd(lcd, LCD_CMD_SET_CGRAM | offset);
	for (i=0 ; i<rows ; i++){
		lcd_data(lcd, bitmap[i]);
		_platform_delay_ms(1);
	}
	lcd_cmd(lcd, LCD_CMD_RETURN_HOME);
	_platform_delay_ms(1);
}

void lcd_backlight_set (lcd_t *lcd, uint8_t state){
	_platform_gpio_write(lcd->backlightGpio, (state > 0));
}

void lcd_backlight_set_bright (lcd_t *lcd, uint8_t level){
	_platform_pwm_control(lcd->backlightPwm, level);
}
