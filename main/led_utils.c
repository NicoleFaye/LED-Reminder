#include "led_utils.h"

// Global array of LED settings
LEDSettings led_settings[NUM_LEDS] = {0};

// Initialize LED GPIO pins
void initialize_led(void)
{
    int led_pins[NUM_LEDS] = {LED1, LED2, LED3, LED4, LED5};
    
    for (int i = 0; i < NUM_LEDS; i++) {
        gpio_reset_pin(led_pins[i]);
        gpio_set_direction(led_pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(led_pins[i], 0);
        
        led_settings[i].pin = led_pins[i];
        led_settings[i].active = false;
        strcpy(led_settings[i].function_mode, "manual");
        strcpy(led_settings[i].display_mode, "solid");
    }

    // Create the LED management task
    xTaskCreate(led_task, "led_task", 2048, NULL, 5, NULL);
}

void set_led_state(int led_index, bool state)
{
    if (led_index >= 0 && led_index < NUM_LEDS) {
        led_settings[led_index].active = state;
        
        // For manual mode, we'll let the LED task handle it
        if (strcmp(led_settings[led_index].function_mode, "manual") == 0) {
            // The LED task will handle the actual GPIO control
        }
    }
}

void led_task(void *pvParameters)
{
    TickType_t last_wake_time;
    const TickType_t frequency = pdMS_TO_TICKS(50); // Update every 50ms
    last_wake_time = xTaskGetTickCount();

    TickType_t led_timers[NUM_LEDS] = {0};
    int led_fade_direction[NUM_LEDS] = {1}; // 1 for fade in, -1 for fade out
    int led_brightness[NUM_LEDS] = {0};

    while (1)
    {
        TickType_t now = xTaskGetTickCount();

        for (int i = 0; i < NUM_LEDS; i++)
        {
            bool should_be_on = led_settings[i].active;
            /*
            bool should_be_on = false;

            // Check function mode
            if (strcmp(led_settings[i].function_mode, "manual") == 0)
            {
                should_be_on = led_settings[i].active;
            }
            //TODO decide if these other clauses are needed or if a separate task should be created for timing of each led
            else if (strcmp(led_settings[i].function_mode, "offset") == 0)
            {
                time_t current_time;
                time(&current_time);
                should_be_on = (current_time % led_settings[i].offset_seconds) < (led_settings[i].offset_seconds / 2) || led_settings[i].active;
            }
            else if (strcmp(led_settings[i].function_mode, "set_time") == 0)
            {
                time_t current_time;
                struct tm timeinfo;
                time(&current_time);
                localtime_r(&current_time, &timeinfo);
                should_be_on = (timeinfo.tm_yday % led_settings[i].set_time_days) == 0;
            }
            else if (strcmp(led_settings[i].function_mode, "fixed_interval") == 0)
            {
                should_be_on = ((now / pdMS_TO_TICKS(1000)) % led_settings[i].fixed_interval_seconds) < (led_settings[i].fixed_interval_seconds / 2);
            }
            */

            // Apply display mode
            if (strcmp(led_settings[i].display_mode, "solid") == 0)
            {
                gpio_set_level(led_settings[i].pin, should_be_on);
            }
            else if (strcmp(led_settings[i].display_mode, "blink") == 0)
            {
                if (should_be_on)
                {
                    if ((now - led_timers[i]) >= pdMS_TO_TICKS(led_settings[i].blink_rate))
                    {
                        led_settings[i].active = !led_settings[i].active;
                        gpio_set_level(led_settings[i].pin, led_settings[i].active);
                        led_timers[i] = now;
                    }
                }
                else
                {
                    gpio_set_level(led_settings[i].pin, 0);
                }
            }
            else if (strcmp(led_settings[i].display_mode, "fade") == 0)
            {
                if (should_be_on)
                {
                    if ((now - led_timers[i]) >= pdMS_TO_TICKS(led_settings[i].fade_rate))
                    {
                        led_brightness[i] += led_fade_direction[i];
                        if (led_brightness[i] >= led_settings[i].brightness || led_brightness[i] <= 0)
                        {
                            led_fade_direction[i] *= -1;
                        }
                        // Implement PWM or analog write here for fading effect
                        // For simplicity, we'll just set the LED on/off based on brightness
                        gpio_set_level(led_settings[i].pin, led_brightness[i] > 0);
                        led_timers[i] = now;
                    }
                }
                else
                {
                    gpio_set_level(led_settings[i].pin, 0);
                }
            }

            // Handle blink sequence if active
            if (led_settings[i].blink_sequence.active)
            {
                static int blink_count[NUM_LEDS] = {0};
                if ((now - led_timers[i]) >= pdMS_TO_TICKS(led_settings[i].blink_sequence.delay_ms))
                {
                    led_settings[i].active = !led_settings[i].active;
                    gpio_set_level(led_settings[i].pin, led_settings[i].active);
                    led_timers[i] = now;
                    if (!led_settings[i].active)
                    {
                        blink_count[i]++;
                        if (blink_count[i] >= led_settings[i].blink_sequence.num_blinks)
                        {
                            led_settings[i].blink_sequence.active = false;
                            blink_count[i] = 0;
                        }
                    }
                }
            }
        }

        vTaskDelayUntil(&last_wake_time, frequency);
    }
}

void blink(int delay_ms, int num_blinks, int led_index)
{
    if (led_index >= 0 && led_index < NUM_LEDS)
    {
        led_settings[led_index].blink_sequence = (BlinkSequence){delay_ms, num_blinks, true};
    }
}

void blinkSet(int delay_ms, int num_blinks, int led_indices[], int num_leds)
{
    for (int i = 0; i < num_leds; i++)
    {
        if (led_indices[i] >= 0 && led_indices[i] < NUM_LEDS)
        {
            led_settings[led_indices[i]].blink_sequence = (BlinkSequence){delay_ms, num_blinks, true};
        }
    }
}