/*
 * LCD1602.c
 *
 *  Created on: 21-Jan-2020
 *      Author: Controllerstech
 */

#include "lcdDisplay.h"


/****************************************************************************************************************************************************************/
static void _delay_us(lcd_t *lcd, uint32_t us){
	lcd->DelayUsFxn(us);
}

static void _delay_ms(lcd_t *lcd, uint8_t ms){
	lcd->DelayUsFxn(ms*1000);
}

static void _send_bit_to_pin(lcd_t *lcd, lcd_pin_e e, uint8_t val){
	lcd->GpioFxn(e, val);
}

static void _send_to_lcd (lcd_t *lcd, char data, int rs)
{
	uint8_t _dataSend[2], steps, i;

	_send_bit_to_pin(lcd, LCD_RS, rs);  // rs = 1 for data, rs=0 for command

	_delay_us(lcd, 5);
	if (lcd->interface == LCD_INTERFACE_4BIT){
		// will send the dara byte in two steps
		// first is sent the Upper 4 bits (higher nibble) over the interface
		// in the second step, is sent the lower nibble
		// if is the first 3 steps, ignore lower nibles
		if (lcd->_initialized == 0){
			steps = 1;
		}
		else{
			steps = 2;
		}
		_dataSend[0] = ((data >> 4) & 0xF);
		_dataSend[1] = (data & 0xF);
		for (i=0 ; i<steps ; i++){
			/* write the data to the respective pin */
			_send_bit_to_pin(lcd, LCD_D7, ((_dataSend[i]>>3)&0x01));
			_send_bit_to_pin(lcd, LCD_D6, ((_dataSend[i]>>2)&0x01));
			_send_bit_to_pin(lcd, LCD_E, 1);
			_send_bit_to_pin(lcd, LCD_D5, ((_dataSend[i]>>1)&0x01));
			_send_bit_to_pin(lcd, LCD_D4, ((_dataSend[i]>>0)&0x01));

			/* Toggle EN PIN to send the data */
			_send_bit_to_pin(lcd, LCD_E, 0);
		}
	}
	else{
		_dataSend[0] = data;

		/* write the data to the respective pin */
		_send_bit_to_pin(lcd, LCD_D7, ((_dataSend[0]>>7)&0x01));
		_send_bit_to_pin(lcd, LCD_D6, ((_dataSend[0]>>6)&0x01));
		_send_bit_to_pin(lcd, LCD_D5, ((_dataSend[0]>>5)&0x01));
		_send_bit_to_pin(lcd, LCD_D4, ((_dataSend[0]>>4)&0x01));
		_send_bit_to_pin(lcd, LCD_E, 1);
		_send_bit_to_pin(lcd, LCD_D3, ((_dataSend[0]>>3)&0x01));
		_send_bit_to_pin(lcd, LCD_D2, ((_dataSend[0]>>2)&0x01));
		_send_bit_to_pin(lcd, LCD_D1, ((_dataSend[0]>>1)&0x01));
		_send_bit_to_pin(lcd, LCD_D0, ((_dataSend[0]>>0)&0x01));

		/* Toggle EN PIN to send the data
		 */
		_send_bit_to_pin(lcd, LCD_E, 0);
	}
}

static void _lcd_cmd(lcd_t *lcd, lcd_cmd_e cmd){
	_send_to_lcd(lcd, (uint8_t)cmd, LCD_RS_OFF);
}

static void _lcd_data(lcd_t *lcd, uint8_t data){
	_send_to_lcd(lcd, (uint8_t)data, LCD_RS_ON);
}

/**
 * Publics
 */

void lcd_init (lcd_t *lcd, lcd_params_t *params)
{
	uint8_t dispDl, dispFont;

	assert(lcd != NULL);
	assert(params != NULL);
	assert(params->GpioFxn != NULL);
	assert(params->columns > 0);
	assert(params->rows > 0);
	assert(params->interface <= LCD_INTERFACE_8BIT);
	assert(params->font <= LCD_FONT_5X10);

	lcd->BacklightFxn = params->BacklightFxn;
	lcd->GpioFxn = params->GpioFxn;
	lcd->columns = params->columns;
	lcd->rows = params->rows;
	lcd->interface = params->BacklightFxn;
	lcd->font = params->font;
	lcd->_initialized = 0;

	if (lcd->font == LCD_FONT_5X8){
		dispFont = DISPLAY_FONT_5x8;
	}
	else{
		dispFont = DISPLAY_FONT_5x10;
	}
	_delay_ms(lcd, 40);  	// wait for >40ms
	_lcd_cmd (lcd, (lcd_cmd_e)(LCD_CMD_FUNCTION_SET | DISPLAY_DL_8BIT));
	_delay_ms(lcd, 10);  	// wait for >4.1ms
	_lcd_cmd (lcd, (lcd_cmd_e)(LCD_CMD_FUNCTION_SET | DISPLAY_DL_8BIT));
	_delay_us(lcd, 100);  	// wait for >100us
	_lcd_cmd (lcd, (lcd_cmd_e)(LCD_CMD_FUNCTION_SET | DISPLAY_DL_8BIT));
	_delay_us(lcd, 100);	// wait for >100us
	if (lcd->interface == LCD_INTERFACE_4BIT){
		// 4 bit initialisation
		_lcd_cmd (lcd, (lcd_cmd_e)(LCD_CMD_FUNCTION_SET | DISPLAY_DL_4BIT));  // 4bit mode
		dispDl = DISPLAY_DL_4BIT;
	}
	else{
		_lcd_cmd (lcd, (lcd_cmd_e)(LCD_CMD_FUNCTION_SET | DISPLAY_DL_8BIT));
		dispDl = DISPLAY_DL_8BIT;
	}
	_delay_ms(lcd, 10);
	lcd->_initialized = 1;

  // dislay initialisation
	lcd->_cursor = DISPLAY_CONTROL_ON | DISPLAY_CURSOR_OFF | DISPLAY_BLINK_OFF;
	_lcd_cmd (lcd, (lcd_cmd_e)(LCD_CMD_FUNCTION_SET | dispDl | DISPLAY_N_2LINE | dispFont)); // Function set --> DL=0 or 1, N = 1 (2 line display) F = 0 (5x8 characters)
	_delay_ms(lcd, 5);
	_lcd_cmd (lcd, LCD_CMD_DISP_CONTROL); //Display on/off control --> D=0,C=0, B=0  ---> display off
	_delay_ms(lcd, 5);
	_lcd_cmd (lcd, LCD_CMD_CLEAR);  // clear display
	_delay_ms(lcd, 5);
	_lcd_cmd (lcd, LCD_CMD_INC_CURSOR); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	_delay_ms(lcd, 5);
	_lcd_cmd (lcd, (lcd_cmd_e)(LCD_CMD_DISP_CONTROL | lcd->_cursor)); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)

	_lcd_cmd(lcd, LCD_CMD_RETURN_HOME);
	lcd->_column = 0;
	lcd->_row = 0;
}

void lcd_blink_off(lcd_t *lcd){
	assert(lcd != NULL && lcd->_initialized == 1);

	lcd->_cursor &= ~(DISPLAY_BLINK_ON);
	lcd_cmd(lcd, (lcd_cmd_e)(LCD_CMD_DISP_CONTROL | lcd->_cursor));
}

void lcd_blink_on(lcd_t *lcd){
	assert(lcd != NULL && lcd->_initialized == 1);

	lcd->_cursor |= (DISPLAY_BLINK_ON);
	lcd_cmd(lcd, (lcd_cmd_e)(LCD_CMD_DISP_CONTROL | lcd->_cursor));
}

void lcd_cursor_off(lcd_t *lcd){
	assert(lcd != NULL && lcd->_initialized == 1);

	lcd->_cursor &= ~(DISPLAY_CURSOR_ON);
	lcd_cmd(lcd, (lcd_cmd_e)(LCD_CMD_DISP_CONTROL | lcd->_cursor));
}

void lcd_cursor_on(lcd_t *lcd){
	assert(lcd != NULL && lcd->_initialized == 1);

	lcd->_cursor |= (DISPLAY_CURSOR_ON);
	lcd_cmd(lcd, (lcd_cmd_e)(LCD_CMD_DISP_CONTROL | lcd->_cursor));
}

void lcd_display_off(lcd_t *lcd){
	assert(lcd != NULL && lcd->_initialized == 1);

	lcd->_cursor &= ~(DISPLAY_CONTROL_ON);
	lcd_cmd(lcd, (lcd_cmd_e)(LCD_CMD_DISP_CONTROL | lcd->_cursor));
}

void lcd_display_on(lcd_t *lcd){
	assert(lcd != NULL && lcd->_initialized == 1);

	lcd->_cursor |= (DISPLAY_CONTROL_ON);
	lcd_cmd(lcd, (lcd_cmd_e)(LCD_CMD_DISP_CONTROL | lcd->_cursor));
}

void lcd_cmd (lcd_t *lcd, lcd_cmd_e cmd)
{
	assert(lcd != NULL && lcd->_initialized == 1);

	_lcd_cmd(lcd, cmd);
}

void lcd_data (lcd_t *lcd, uint8_t data){
	assert(lcd != NULL && lcd->_initialized == 1);

	_lcd_data(lcd, data);
}

void lcd_put_pos(lcd_t *lcd, uint8_t row, uint8_t col)
{
	assert(lcd != NULL && lcd->_initialized == 1);

	if (row >= lcd->rows || col >= lcd->columns){
		return;
	}
    lcd->_column = col;
    lcd->_row = row;
    switch (row)
    {
        case 0:
            col += 0x80;
            break;
        case 1:
            col += 0xC0;
            break;
        case 2:
        	col += 0x94;
        	break;
        case 3:
        	col += 0xD4;
        	break;
    }
    _lcd_cmd (lcd, (lcd_cmd_e)col);
}

void lcd_send_char (lcd_t *lcd, char data){
	assert(lcd != NULL && lcd->_initialized == 1);

	if (lcd->_column >= lcd->columns || lcd->_row >= lcd->rows){
		return;
	}
	_lcd_data(lcd, (uint8_t)data);
	lcd->_column++;
}

void lcd_send_string (lcd_t *lcd, char *str)
{
	assert(lcd != NULL && lcd->_initialized == 1);
	assert(str =! NULL);

	while (*str)lcd_send_char (lcd, (*str++));
}

void lcd_send_char_pos(lcd_t *lcd, char data, uint8_t row, uint8_t col){
	assert(lcd != NULL && lcd->_initialized == 1);

	lcd_put_pos(lcd, row, col);
	lcd_send_char(lcd, data);
}

void lcd_send_string_pos (lcd_t *lcd, char *str, uint8_t row, uint8_t col){
	assert(lcd != NULL && lcd->_initialized == 1);

	lcd_put_pos(lcd, row, col);
	lcd_send_string(lcd, str);
}

void lcd_clear_row (lcd_t *lcd, uint8_t row){
	assert(lcd != NULL && lcd->_initialized == 1);

	uint8_t i;

	if (row < lcd->rows){
		lcd_put_pos(lcd, row, 0);
		for (i=0 ; i<lcd->columns ; i++){
			lcd_send_char(lcd, ' ');
		}
	}
}

void lcd_clear_all (lcd_t *lcd){
	assert(lcd != NULL && lcd->_initialized == 1);

	lcd_cmd(lcd, LCD_CMD_CLEAR);
	_delay_ms(lcd, 2);
}

void lcd_create_custom_char (lcd_t *lcd, lcd_custom_char_e custom, uint8_t *bitmap){
	assert(lcd != NULL && lcd->_initialized == 1);
	assert(bitmap != NULL);

	uint8_t i, rows, offset;

	if (lcd->font == LCD_FONT_5X8){
		rows = 8;
	}
	else{
		rows = 10;
	}
	offset = custom*0x8;
	lcd_cmd(lcd, (lcd_cmd_e)(LCD_CMD_SET_CGRAM | offset));
	for (i=0 ; i<rows ; i++){
		lcd_data(lcd, bitmap[i]);
		_delay_ms(lcd, 1);
	}
	lcd_cmd(lcd, LCD_CMD_RETURN_HOME);
	_delay_ms(lcd, 1);
}

void lcd_backlight_set (lcd_t *lcd, uint8_t state){
	assert(lcd != NULL && lcd->_initialized == 1);

	if (lcd->BacklightFxn != NULL){
		if (state == 0){
			lcd->BacklightFxn(0);
		}
		else {
			lcd->BacklightFxn(100);
		}
	}
}

void lcd_backlight_set_bright (lcd_t *lcd, uint8_t level){
	assert(lcd != NULL && lcd->_initialized == 1);

	if (lcd->BacklightFxn != NULL){
		lcd->BacklightFxn(level);
	}
}
