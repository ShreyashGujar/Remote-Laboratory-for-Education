#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"

#define RELAY_PIN 2
#define BUTTON_PIN 4

#define LCD_RS 19
#define LCD_EN 18
#define LCD_D4 5
#define LCD_D5 17
#define LCD_D6 16
#define LCD_D7 15

static const char *TAG = "TIMER";

uint8_t hour = 0;
uint8_t minute = 0;
uint8_t second = 0;

uint8_t on_hour = 8;
uint8_t on_minute = 0;
uint8_t off_hour = 18;
uint8_t off_minute = 0;

uint8_t relay_state = 0;

void delay_us(uint32_t us) {
    ets_delay_us(us);
}

void delay_ms(uint32_t ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void lcd_pulse_enable(void) {
    gpio_set_level(LCD_EN, 1);
    delay_us(1);
    gpio_set_level(LCD_EN, 0);
    delay_us(100);
}

void lcd_send_nibble(uint8_t nibble) {
    gpio_set_level(LCD_D4, (nibble >> 0) & 0x01);
    gpio_set_level(LCD_D5, (nibble >> 1) & 0x01);
    gpio_set_level(LCD_D6, (nibble >> 2) & 0x01);
    gpio_set_level(LCD_D7, (nibble >> 3) & 0x01);
    lcd_pulse_enable();
}

void lcd_cmd(uint8_t cmd) {
    gpio_set_level(LCD_RS, 0);
    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);
    delay_ms(2);
}

void lcd_char(uint8_t data) {
    gpio_set_level(LCD_RS, 1);
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
    delay_ms(2);
}

void lcd_print(char *str) {
    while(*str) {
        lcd_char(*str++);
    }
}

void lcd_num(uint8_t num) {
    if(num < 10) lcd_char('0');
    lcd_char((num / 10) + '0');
    lcd_char((num % 10) + '0');
}

void lcd_init(void) {
    gpio_set_direction(LCD_RS, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_EN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D4, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D5, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D6, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D7, GPIO_MODE_OUTPUT);
    
    delay_ms(50);
    lcd_cmd(0x02);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    delay_ms(2);
}

void relay_on(void) {
    gpio_set_level(RELAY_PIN, 1);
    relay_state = 1;
    ESP_LOGI(TAG, "Relay ON");
}

void relay_off(void) {
    gpio_set_level(RELAY_PIN, 0);
    relay_state = 0;
    ESP_LOGI(TAG, "Relay OFF");
}

void check_timer(void) {
    if(hour == on_hour && minute == on_minute && second == 0) {
        relay_on();
    }
    
    if(hour == off_hour && minute == off_minute && second == 0) {
        relay_off();
    }
}

void display_screen(void) {
    lcd_cmd(0x80);
    lcd_print("Time: ");
    lcd_num(hour);
    lcd_char(':');
    lcd_num(minute);
    lcd_char(':');
    lcd_num(second);
    
    lcd_cmd(0xC0);
    lcd_print("Relay: ");
    if(relay_state) {
        lcd_print("ON  ");
    } else {
        lcd_print("OFF ");
    }
}

void timer_task(void *pvParameter) {
    while(1) {
        second++;
        if(second >= 60) {
            second = 0;
            minute++;
        }
        if(minute >= 60) {
            minute = 0;
            hour++;
        }
        if(hour >= 24) {
            hour = 0;
        }
        
        check_timer();
        display_screen();
        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void button_task(void *pvParameter) {
    while(1) {
        if(gpio_get_level(BUTTON_PIN) == 0) {
            if(relay_state) {
                relay_off();
            } else {
                relay_on();
            }
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    
    lcd_init();
    relay_off();
    
    lcd_cmd(0x01);
    lcd_print("Timer Switch");
    lcd_cmd(0xC0);
    lcd_print("ESP32 Ready");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    
    xTaskCreate(&timer_task, "timer_task", 2048, NULL, 5, NULL);
    xTaskCreate(&button_task, "button_task", 2048, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "System started");
}
