/*
 * LCD1602.c
 *
 *  Created on: 21-Jan-2020
 *      Author: Controllerstech
 */

#include "lcdDisplay.h"


void delay (lcd_t *lcd, uint16_t us)
{
	uint32_t ticks = lcd->_ticksForUs*us;

	while (ticks > 0){
		ticks--;
	}
}

/****************************************************************************************************************************************************************/

void send_to_lcd (lcd_t *lcd, char data, int rs)
{
	uint8_t _dataSend[2], steps, i;

	HAL_GPIO_WritePin(lcd->gpios[LCD_RS].GPIO, lcd->gpios[LCD_RS].pin, rs);  // rs = 1 for data, rs=0 for command
	delay(lcd, 5);
	if (lcd->interface == LCD_INTERFACE_4BIT){
		// will send the dara byte in two steps
		// first is sent the Upper 4 bits (higher nibble) over the interface
		// in the second step, is sent the lower nibble
		steps = 2;
		_dataSend[0] = (data >> 4);
		_dataSend[1] = (data & 0xF);
		for (i=0 ; i<steps ; i++){
			/* write the data to the respective pin */
			HAL_GPIO_WritePin(lcd->gpios[LCD_D7].GPIO, lcd->gpios[LCD_D7].pin, ((_dataSend[i]>>3)&0x01));
			HAL_GPIO_WritePin(lcd->gpios[LCD_D6].GPIO, lcd->gpios[LCD_D6].pin, ((_dataSend[i]>>2)&0x01));
			HAL_GPIO_WritePin(lcd->gpios[LCD_D5].GPIO, lcd->gpios[LCD_D5].pin, ((_dataSend[i]>>1)&0x01));
			HAL_GPIO_WritePin(lcd->gpios[LCD_D4].GPIO, lcd->gpios[LCD_D4].pin, ((_dataSend[i]>>0)&0x01));

			/* Toggle EN PIN to send the data
			 * if the HCLK > 100 MHz, use the  20 us delay
			 * if the LCD still doesn't work, increase the delay to 50, 80 or 100..
			 */
			HAL_GPIO_WritePin(lcd->gpios[LCD_E].GPIO, lcd->gpios[LCD_E].pin, 1);
			delay(lcd, 20);
			HAL_GPIO_WritePin(lcd->gpios[LCD_E].GPIO, lcd->gpios[LCD_E].pin, 0);
			delay(lcd, 20);
		}
	}
	else{
		_dataSend[0] = data;
		/* write the data to the respective pin */
		HAL_GPIO_WritePin(lcd->gpios[LCD_D7].GPIO, lcd->gpios[LCD_D7].pin, ((_dataSend[0]>>7)&0x01));
		HAL_GPIO_WritePin(lcd->gpios[LCD_D6].GPIO, lcd->gpios[LCD_D6].pin, ((_dataSend[0]>>6)&0x01));
		HAL_GPIO_WritePin(lcd->gpios[LCD_D5].GPIO, lcd->gpios[LCD_D5].pin, ((_dataSend[0]>>5)&0x01));
		HAL_GPIO_WritePin(lcd->gpios[LCD_D4].GPIO, lcd->gpios[LCD_D4].pin, ((_dataSend[0]>>4)&0x01));
		HAL_GPIO_WritePin(lcd->gpios[LCD_D3].GPIO, lcd->gpios[LCD_D3].pin, ((_dataSend[0]>>3)&0x01));
		HAL_GPIO_WritePin(lcd->gpios[LCD_D2].GPIO, lcd->gpios[LCD_D2].pin, ((_dataSend[0]>>2)&0x01));
		HAL_GPIO_WritePin(lcd->gpios[LCD_D1].GPIO, lcd->gpios[LCD_D1].pin, ((_dataSend[0]>>1)&0x01));
		HAL_GPIO_WritePin(lcd->gpios[LCD_D0].GPIO, lcd->gpios[LCD_D0].pin, ((_dataSend[0]>>0)&0x01));

		/* Toggle EN PIN to send the data
		 * if the HCLK > 100 MHz, use the  20 us delay
		 * if the LCD still doesn't work, increase the delay to 50, 80 or 100..
		 */
		HAL_GPIO_WritePin(lcd->gpios[LCD_E].GPIO, lcd->gpios[LCD_E].pin, 1);
		delay(lcd, 20);
		HAL_GPIO_WritePin(lcd->gpios[LCD_E].GPIO, lcd->gpios[LCD_E].pin, 0);
		delay(lcd, 20);
	}
}

//void lcd_send_data (lcd_t *lcd, char data)
//{
//	char datatosend;
//
//	/* send higher nibble */
//	datatosend = ((data>>4)&0x0f);
//	send_to_lcd(lcd, datatosend, 1);  // rs =1 for sending data
//
//	/* send Lower nibble */
//	datatosend = ((data)&0x0f);
//	send_to_lcd(lcd, datatosend, 1);
//}

/**
 * Publics
 */

void lcd_init (lcd_t *lcd)
{
	uint32_t sysclk = HAL_RCC_GetSysClockFreq();

	lcd->_ticksForUs = (sysclk)/(2E6);

	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_cmd (lcd, LCD_CMD_FUNCTION_SET | DISPLAY_DL_8BIT);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_cmd (lcd, LCD_CMD_FUNCTION_SET | DISPLAY_DL_8BIT);
	HAL_Delay(1);  // wait for >100us
	lcd_cmd (lcd, LCD_CMD_FUNCTION_SET | DISPLAY_DL_8BIT);
	HAL_Delay(10);
	lcd_cmd (lcd, LCD_CMD_FUNCTION_SET | DISPLAY_DL_4BIT);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_cmd (lcd, LCD_CMD_FUNCTION_SET | DISPLAY_DL_4BIT | DISPLAY_N_2LINE | DISPLAY_FONT_5x8); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_cmd (lcd, LCD_CMD_DISPOFF_CURSOROFF); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_cmd (lcd, LCD_CMD_CLEAR);  // clear display
	HAL_Delay(2);
	lcd_cmd (lcd, LCD_CMD_INC_CURSOR); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_cmd (lcd, LCD_CMD_DISPON_CURSOROFF); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)

	lcd_put_pos(lcd, 0, 0);
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
	HAL_Delay(10);
}

void lcd_create_custom_char (lcd_t *lcd, lcd_custom_char_e custom, uint8_t *bitmap){
	uint8_t i;

	lcd_cmd(lcd, LCD_CMD_SET_CGRAM | custom);
	for (i=0 ; i<LCD_CHAR_ROWS ; i++){
		lcd_data(lcd, bitmap[i]);
	}
}
