/*
 * SPDX-FileCopyrightText: 2020-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_rom_sys.h"
#include "rom/ets_sys.h"

#include "lcdDisplay.h"

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO18: output (ESP32C2/ESP32H2 uses GPIO8 as the second output pin)
 * GPIO19: output (ESP32C2/ESP32H2 uses GPIO9 as the second output pin)
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Note. These are the default GPIO pins to be used in the example. You can
 * change IO pins in menuconfig.
 *
 * Test:
 * Connect GPIO18(8) with GPIO4
 * Connect GPIO19(9) with GPIO5
 * Generate pulses on GPIO18(8)/19(9), that triggers interrupt on GPIO4/5
 *
 */

#define LCD_GPIO_PIN_RS         (4)
#define LCD_GPIO_PIN_E          (5) 
#define LCD_GPIO_PIN_D4         (6)
#define LCD_GPIO_PIN_D5         (7)
#define LCD_GPIO_PIN_D6         (9)
#define LCD_GPIO_PIN_D7         (10)

#define LCD_GPIO_PIN_RS_MASK         (1 << LCD_GPIO_PIN_RS)
#define LCD_GPIO_PIN_E_MASK          (1 << LCD_GPIO_PIN_E) 
#define LCD_GPIO_PIN_D4_MASK         (1 << LCD_GPIO_PIN_D4)
#define LCD_GPIO_PIN_D5_MASK         (1 << LCD_GPIO_PIN_D5)
#define LCD_GPIO_PIN_D6_MASK         (1 << LCD_GPIO_PIN_D6)
#define LCD_GPIO_PIN_D7_MASK         (1 << LCD_GPIO_PIN_D7)
#define LCD_GPIO_LCD_PIN_MASK         (LCD_GPIO_PIN_RS_MASK | LCD_GPIO_PIN_E_MASK | LCD_GPIO_PIN_D4_MASK | \
                                       LCD_GPIO_PIN_D5_MASK | LCD_GPIO_PIN_D6_MASK | LCD_GPIO_PIN_D7_MASK)

// LCD Handle
lcd_t Lcd = {0};

/* Initialize the GPIOs */
void app_init_gpio(){
    //zero-initialize the config structure.
    gpio_config_t io_conf = {0};
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = LCD_GPIO_LCD_PIN_MASK;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}

/* Lcd routines*/

void app_lcd_gpio(lcd_pin_e pin, uint8_t state){
    switch (pin) {
        case LCD_RS:
            gpio_set_level(LCD_GPIO_PIN_RS, state);
            break;
        case LCD_E:
            gpio_set_level(LCD_GPIO_PIN_E, state);
            break;
        case LCD_D4:
            gpio_set_level(LCD_GPIO_PIN_D4, state);
            break;
        case LCD_D5:
            gpio_set_level(LCD_GPIO_PIN_D5, state);
            break;
        case LCD_D6:
            gpio_set_level(LCD_GPIO_PIN_D6, state);
            break;
        case LCD_D7:
            gpio_set_level(LCD_GPIO_PIN_D7, state);
            break;
        default:
            break;
    }
}

void app_lcd_delay(uint32_t us){
    if (us < 1000){
        //vTaskDelay(pdMS_TO_TICKS(2));
        ets_delay_us(us);
    }
    else{
        vTaskDelay(pdMS_TO_TICKS(us/1000));
    }
}

void app_init_lcd(){
    lcd_params_t params = {0};

    params.rows = 4;
    params.columns = 20;
    params.interface = LCD_INTERFACE_4BIT;  
    params.font = LCD_FONT_5X8;
    params.GpioFxn = app_lcd_gpio;
    params.DelayUsFxn = app_lcd_delay;
    
    lcd_init(&Lcd, &params);
    lcd_clear_all(&Lcd);

}

void app_main(void)
{
    char Text[32];
    int i = 0;

    vTaskDelay(pdMS_TO_TICKS(1000));
    // Initialize the GPIOs
    app_init_gpio();
    // Initialize the LCD
    app_init_lcd();

    // Send a string to the LCD
    lcd_send_string_pos(&Lcd, "> Hello World from", 0, 0);
    lcd_send_string_pos(&Lcd, "> ESP32-C3", 1, 0);
    lcd_send_string_pos(&Lcd, "> Ouroboros Embedded", 2, 0);
    while (1) {
        // Generate a string with the counter value
        sprintf(Text, "> Counter: %d", i++);
        // Send the string to the LCD
        lcd_send_string_pos(&Lcd, Text, 3, 0);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
