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
#include <time.h>
#include <string.h>
#include "driver/ledc.h"

#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
#define LED5 5

#define NUM_LEDS 5
#define WIFI_CONNECTED_BLINK_DELAY_MS 500

#define PWM_FREQUENCY 5000 // 5 kHz
#define PWM_RESOLUTION LEDC_TIMER_8_BIT // 8-bit resolution, 256 levels

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
    int current_brightness;
    int fade_direction;
    bool blinking;
    BlinkSequence blink_sequence;
    bool active;
    int pin;
    ledc_channel_t pwm_channel;
    TickType_t last_update;
} LEDSettings;

// Global array of LED settings
extern LEDSettings led_settings[NUM_LEDS];

void initialize_led(void);
void set_led_state(int led_index, bool state);
void set_led_brightness(int led_index, int brightness);
void led_task(void *pvParameters);
void blink(int delay_ms, int num_blinks, int led_index);
void blinkSet(int delay_ms, int num_blinks, int led_indices[], int num_leds);

#endif