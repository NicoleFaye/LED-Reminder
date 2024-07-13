#ifndef LED_UTILS_H
#define LED_UTILS_H

#include "settings.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
#define LED5 5

#define NUM_LEDS 5
#define WIFI_CONNECTED_BLINK_DELAY_MS 500

// Blink sequence structure
typedef struct {
    int delay_ms;
    int num_blinks;
    bool active;
} BlinkSequence;

// LED Settings structure
typedef struct {
    char function_mode[20];
    int offset_seconds;
    int set_time_days;
    int fixed_interval_seconds;
    char display_mode[10];
    int blink_rate;
    int fade_rate;
    int brightness;
    BlinkSequence blink_sequence;
} LEDSettings;

// Global array of LED settings
extern LEDSettings led_settings[NUM_LEDS];

void initialize_led(void);
void set_led_state(int led_pin, bool state);
void led_task(void *pvParameters);
void blink(int delay_ms, int num_blinks, int pin);
void blinkSet(int delay_ms, int num_blinks, int pins[], int num_pins);

#endif