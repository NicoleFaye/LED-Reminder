#ifndef BUTTONS_H
#define BUTTONS_H


#include "settings.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "led_utils.h"

#define BUTTON1 7
#define BUTTON2 8
#define BUTTON3 9
#define BUTTON4 10
#define BUTTON5 11

#define NUM_BUTTONS 5

#define BUTTON_LONG_PRESS_DELAY_MS 1500

void initialize_buttons(void);
bool get_button_state(int pin);

#endif