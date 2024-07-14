#include "led_utils.h"

// Global array of LED settings
LEDSettings led_settings[NUM_LEDS] = {0};
void initialize_ledc(void)
{
    // Configure LEDC timer
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = PWM_RESOLUTION,
        .freq_hz = PWM_FREQUENCY,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);

    // Configure LEDC channels for each LED
    for (int i = 0; i < NUM_LEDS; i++) {
        ledc_channel_config_t ledc_channel = {
            .channel = led_settings[i].pwm_channel,
            .duty = 0,
            .gpio_num = led_settings[i].pin,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .timer_sel = LEDC_TIMER_0
        };
        ledc_channel_config(&ledc_channel);
    }
}


void initialize_led(void)
{
    int led_pins[NUM_LEDS] = {LED1, LED2, LED3, LED4, LED5};
    
    for (int i = 0; i < NUM_LEDS; i++) {
        gpio_reset_pin(led_pins[i]);
        gpio_set_direction(led_pins[i], GPIO_MODE_OUTPUT);
        
        led_settings[i].pin = led_pins[i];
        led_settings[i].active = false;
        strcpy(led_settings[i].function_mode, "manual");
        strcpy(led_settings[i].display_mode, "solid");
        led_settings[i].brightness = 100; // Default to full brightness
        led_settings[i].pwm_channel = i; // Assign a unique PWM channel to each LED
    }

    // Initialize LEDC
    initialize_ledc();

    // Create the LED management task
    xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);
}

void set_led_brightness(int led_index, int brightness)
{
    if (led_index >= 0 && led_index < NUM_LEDS) {
        if (brightness <= 0) {
            brightness = 1;
        }
        led_settings[led_index].brightness = brightness;
        int duty = (brightness * ((1 << PWM_RESOLUTION) - 1)) / 100; // Convert percentage to duty cycle
        ledc_set_duty(LEDC_LOW_SPEED_MODE, led_settings[led_index].pwm_channel, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, led_settings[led_index].pwm_channel);
        //ESP_LOGI(TAG, "LED %d brightness set to %d, duty: %d", led_index, brightness, duty);
    }
}

void set_led_state(int led_index, bool state)
{
    if (led_index >= 0 && led_index < NUM_LEDS) {
        led_settings[led_index].active = state;
        
        if (state) {
            int duty = (led_settings[led_index].brightness * ((1 << PWM_RESOLUTION) - 1)) / 100;
            ledc_set_duty(LEDC_LOW_SPEED_MODE, led_settings[led_index].pwm_channel, duty);
        } else {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, led_settings[led_index].pwm_channel, 0);
        }
        ledc_update_duty(LEDC_LOW_SPEED_MODE, led_settings[led_index].pwm_channel);
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

if (led_settings[i].blink_sequence.active)
{
    if ((now - led_timers[i]) >= pdMS_TO_TICKS(led_settings[i].blink_sequence.delay_ms))
    {
        should_be_on = !should_be_on;
        set_led_state(i, should_be_on);
        led_timers[i] = now;
        if (should_be_on == false) // Only decrement when turning off
        {
            led_settings[i].blink_sequence.num_blinks--;
            if (led_settings[i].blink_sequence.num_blinks <= 0)
            {
                led_settings[i].blink_sequence.active = false;
                set_led_state(i, led_settings[i].active);  // Return to normal state
            }
        }
    }
}

            // Apply display mode
            if (strcmp(led_settings[i].display_mode, "solid") == 0)
            {
                set_led_state(i, should_be_on);
            }
            else if (strcmp(led_settings[i].display_mode, "blink") == 0)
            {
                if (should_be_on)
                {
                    if ((now - led_timers[i]) >= pdMS_TO_TICKS(led_settings[i].blink_rate))
                    {
                        led_settings[i].active = !led_settings[i].active;
                        set_led_state(i, led_settings[i].active);
                        led_timers[i] = now;
                    }
                }
                else
                {
                    set_led_state(i, false);
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
                        int duty = (led_brightness[i] * 255) / 100;
                        ledc_set_duty(LEDC_LOW_SPEED_MODE, led_settings[i].pwm_channel, duty);
                        ledc_update_duty(LEDC_LOW_SPEED_MODE, led_settings[i].pwm_channel);
                        led_timers[i] = now;
                    }
                }
                else
                {
                    set_led_state(i, false);
                }
            }

            // Handle other modes (offset, set_time, fixed_interval) here
            // ...
        }

        vTaskDelayUntil(&last_wake_time, frequency);
    }
}

void blink(int delay_ms, int num_blinks, int led_index)
{
    if (led_index >= 0 && led_index < NUM_LEDS)
    {
        led_settings[led_index].blink_sequence = (BlinkSequence){delay_ms, num_blinks, true}; 
        led_settings[led_index].active = false;  // Start with LED off
        set_led_state(led_index, false);  // Ensure LED is initially off
    }
}

void blinkSet(int delay_ms, int num_blinks, int led_indices[], int num_leds)
{
    for (int i = 0; i < num_leds; i++)
    {
        blink(delay_ms, num_blinks, led_indices[i]);
    }
}