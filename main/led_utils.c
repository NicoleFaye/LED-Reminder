#include "led_utils.h"

LEDSettings led_settings[NUM_LEDS] = {0};

void initialize_ledc(void)
{
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = PWM_RESOLUTION,
        .freq_hz = PWM_FREQUENCY,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);

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
        led_settings[i].brightness = 100;
        led_settings[i].current_brightness = 0;
        led_settings[i].fade_direction = 1;
        led_settings[i].pwm_channel = i;
        led_settings[i].blink_rate = 500;
        led_settings[i].fade_rate = 50;
        led_settings[i].blink_sequence = (BlinkSequence){0, 0, false};
        led_settings[i].last_update = 0;
    }

    initialize_ledc();
    xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);
}

void set_led_brightness(int led_index, int brightness)
{
    if (led_index >= 0 && led_index < NUM_LEDS) {
        led_settings[led_index].brightness = (brightness > 0) ? brightness : 1;
    }
}

void set_led_state(int led_index, bool state)
{
    if (led_index >= 0 && led_index < NUM_LEDS) {
        led_settings[led_index].active = state;
    }
}

void update_led_duty(int led_index, int brightness)
{
    int duty = (brightness * ((1 << PWM_RESOLUTION) - 1)) / 100;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, led_settings[led_index].pwm_channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, led_settings[led_index].pwm_channel);
}

void led_task(void *pvParameters)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t frequency = pdMS_TO_TICKS(10);

    while (1) {
        for (int i = 0; i < NUM_LEDS; i++) {
            LEDSettings *led = &led_settings[i];
            TickType_t now = xTaskGetTickCount();
            
            if (led->blink_sequence.active) {
                if ((now - led->last_update) >= pdMS_TO_TICKS(led->blink_sequence.delay_ms)) {
                    led->active = !led->active;
                    led->last_update = now;
                    if (!led->active) {
                        led->blink_sequence.num_blinks--;
                        if (led->blink_sequence.num_blinks <= 0) {
                            led->blink_sequence.active = false;
                            led->active = false;
                        }
                    }
                }
                update_led_duty(i, led->active ? led->brightness : 0);
            } else if (led->active) {
                if (strcmp(led->display_mode, "solid") == 0) {
                    update_led_duty(i, led->brightness);
                } else if (strcmp(led->display_mode, "blink") == 0) {
                    if ((now - led->last_update) >= pdMS_TO_TICKS(led->blink_rate)) {
                        led->blinking = !led->blinking;
                        led->last_update = now;
                    }
                    update_led_duty(i, led->blinking ? led->brightness : 0);
                } else if (strcmp(led->display_mode, "fade") == 0) {
                    if ((now - led->last_update) >= pdMS_TO_TICKS(led->fade_rate)) {
                        led->current_brightness += led->fade_direction;
                        if (led->current_brightness >= led->brightness || led->current_brightness <= 0) {
                            led->fade_direction *= -1;
                        }
                        led->last_update = now;
                    }
                    update_led_duty(i, led->current_brightness);
                }
            } else {
                update_led_duty(i, 0);
                led->current_brightness = 0;
                led->fade_direction = 1;
                led->blinking = false;
            }
        }
        vTaskDelayUntil(&last_wake_time, frequency);
    }
}

void blink(int delay_ms, int num_blinks, int led_index)
{
    if (led_index >= 0 && led_index < NUM_LEDS) {
        led_settings[led_index].blink_sequence = (BlinkSequence){delay_ms, num_blinks, true};
        led_settings[led_index].active = true;
        led_settings[led_index].last_update = xTaskGetTickCount();
    }
}

void blinkSet(int delay_ms, int num_blinks, int led_indices[], int num_leds)
{
    for (int i = 0; i < num_leds; i++) {
        blink(delay_ms, num_blinks, led_indices[i]);
    }
}