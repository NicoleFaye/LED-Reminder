#include "led_utils.h"

LEDSettings led_settings[NUM_LEDS] = {0};

void initialize_ledc(void)
{
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = PWM_RESOLUTION,
        .freq_hz = PWM_FREQUENCY,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0};
    ledc_timer_config(&ledc_timer);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        ledc_channel_config_t ledc_channel = {
            .channel = led_settings[i].pwm_channel,
            .duty = 0,
            .gpio_num = led_settings[i].pin,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .timer_sel = LEDC_TIMER_0};
        ledc_channel_config(&ledc_channel);
    }
}

void led_offset_task(void *pvParameters)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t frequency = pdMS_TO_TICKS(100); // Check every 100 ms

    while (1)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            LEDSettings *led = &led_settings[i];
            
            if (strcmp(led->function_mode, "offset") == 0)
            {
                if (!led->active)
                {
                    // If LED is off, check if it's time to turn it on
                    if ((xTaskGetTickCount() - led->last_update) >= pdMS_TO_TICKS(led->offset_seconds * 1000))
                    {
                        set_led_state(i, true);
                        ESP_LOGI(TAG, "LED %d turned on after offset", i+1);
                    }
                }
            }
        }

        vTaskDelayUntil(&last_wake_time, frequency);
    }
}

void led_fixed_interval_task(void *pvParameters)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t frequency = pdMS_TO_TICKS(1000); // Check every second

    while (1)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            LEDSettings *led = &led_settings[i];
            
            if (strcmp(led->function_mode, "fixed_interval") == 0)
            {
                // Check if it's time to toggle the LED
                if ((xTaskGetTickCount() - led->last_update) >= pdMS_TO_TICKS(led->fixed_interval_seconds * 1000))
                {
                    set_led_state(i, true);
                    led->last_update = xTaskGetTickCount();
                    ESP_LOGI(TAG, "LED %d toggled at fixed interval", i+1);
                }
            }
        }

        vTaskDelayUntil(&last_wake_time, frequency);
    }
}

void led_set_time_task(void *pvParameters)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t frequency = pdMS_TO_TICKS(1000); // Check every second

    while (1)
    {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);

        for (int i = 0; i < NUM_LEDS; i++)
        {
            LEDSettings *led = &led_settings[i];
            
            if (strcmp(led->function_mode, "set_time") == 0)
            {
                // Check if it's time to turn on the LED
                if (timeinfo.tm_hour == led->set_time_hours && 
                    timeinfo.tm_min == led->set_time_minutes &&
                    timeinfo.tm_sec == 0)
                {
                    // Check if enough days have passed since last activation
                    if (difftime(now, led->last_on_time) >= (led->set_time_days * 24 * 60 * 60))
                    {
                        set_led_state(i, true);
                        led->last_on_time = now;
                        ESP_LOGI(TAG, "LED %d turned on at set time", i+1);
                    }
                }
            }
        }

        vTaskDelayUntil(&last_wake_time, frequency);
    }
}

void initialize_led(void)
{
    int led_pins[NUM_LEDS] = {LED1, LED2, LED3, LED4, LED5};

    for (int i = 0; i < NUM_LEDS; i++)
    {
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
        led_settings[i].last_on_time = 0;
    }

    initialize_ledc();
    xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);
    xTaskCreate(led_set_time_task, "led_set_time_task", 4096, NULL, 5, NULL);
    xTaskCreate(led_offset_task, "led_offset_task", 4096, NULL, 5, NULL);
    xTaskCreate(led_fixed_interval_task, "led_fixed_interval_task", 4096, NULL, 5, NULL);
}

void set_led_brightness(int led_index, int brightness)
{
    if (led_index >= 0 && led_index < NUM_LEDS)
    {
        led_settings[led_index].brightness = (brightness > 0) ? brightness : 1;
    }
}

void update_led_duty(int led_index, int brightness)
{
    int duty = (brightness * ((1 << PWM_RESOLUTION) - 1)) / 100;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, led_settings[led_index].pwm_channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, led_settings[led_index].pwm_channel);
}

void set_led_state(int led_index, bool state)
{
    if (led_index >= 0 && led_index < NUM_LEDS)
    {
        LEDSettings *led = &led_settings[led_index];
        led->active = state;

        if (!state && strcmp(led->function_mode, "offset") == 0)
        {
            // Reset the timer when the LED is turned off in offset mode
            led->last_update = xTaskGetTickCount();
        }

        // Update the LED's physical state
        update_led_duty(led_index, state ? led->brightness : 0);

        ESP_LOGI(TAG, "LED %d state set to %s", led_index + 1, state ? "ON" : "OFF");
    }
    else
    {
        ESP_LOGE(TAG, "Invalid LED index: %d", led_index);
    }
}


static bool should_sync_leds(int led1, int led2)
{
    LEDSettings *led1_settings = &led_settings[led1];
    LEDSettings *led2_settings = &led_settings[led2];

    bool blink_sync = (strcmp(led1_settings->display_mode, "blink") == 0 &&
                       strcmp(led2_settings->display_mode, "blink") == 0 &&
                       led1_settings->blink_rate == led2_settings->blink_rate);

    bool fade_sync = (strcmp(led1_settings->display_mode, "fade") == 0 &&
                      strcmp(led2_settings->display_mode, "fade") == 0 &&
                      led1_settings->fade_rate == led2_settings->fade_rate);

    return (led1_settings->active && led2_settings->active &&
            ((blink_sync && led1_settings->blinking != led2_settings->blinking) ||
             (fade_sync && led1_settings->current_brightness != led2_settings->current_brightness)));
}

void led_task(void *pvParameters)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t frequency = pdMS_TO_TICKS(10);

    while (1)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            LEDSettings *led = &led_settings[i];
            TickType_t now = xTaskGetTickCount();

            if (led->blink_sequence.active)
            {
                if ((now - led->last_update) >= pdMS_TO_TICKS(led->blink_sequence.delay_ms))
                {
                    led->active = !led->active;
                    led->last_update = now;
                    if (!led->active)
                    {
                        led->blink_sequence.num_blinks--;
                        if (led->blink_sequence.num_blinks <= 0)
                        {
                            led->blink_sequence.active = false;
                            led->active = false;
                        }
                    }
                }
                update_led_duty(i, led->active ? led->brightness : 0);
            }
            else if (led->active)
            {
                if (strcmp(led->display_mode, "solid") == 0)
                {
                    update_led_duty(i, led->brightness);
                }
                else if (strcmp(led->display_mode, "blink") == 0)
                {
                    // Check if this LED should be synced with any other LED
                    bool synced = false;
                    for (int j = 0; j < i && !synced; j++)
                    {
                        if (should_sync_leds(i, j))
                        {
                            led->blinking = led_settings[j].blinking;
                            led->last_update = led_settings[j].last_update;
                            synced = true;
                        }
                    }

                    // If not synced with any previous LED, proceed with normal blinking
                    if (!synced && (now - led->last_update) >= pdMS_TO_TICKS(led->blink_rate))
                    {
                        led->blinking = !led->blinking;
                        led->last_update = now;
                    }
                    update_led_duty(i, led->blinking ? led->brightness : 0);
                }
                else if (strcmp(led->display_mode, "fade") == 0)
                {
                    // Check if this LED should be synced with any other LED
                    bool synced = false;
                    for (int j = 0; j < i && !synced; j++)
                    {
                        if (should_sync_leds(i, j))
                        {
                            led->current_brightness = led_settings[j].current_brightness;
                            led->fade_direction = led_settings[j].fade_direction;
                            led->last_update = led_settings[j].last_update;
                            synced = true;
                        }
                    }

                    // If not synced with any previous LED, proceed with normal fading
                    if (!synced && (now - led->last_update) >= pdMS_TO_TICKS(led->fade_rate))
                    {
                        led->current_brightness += led->fade_direction;
                        // Check if brightness is at or below 5% or at or above 100%
                        if (led->current_brightness >= led->brightness || led->current_brightness <= (0.03 * led->brightness))
                        {
                            led->fade_direction *= -1;
                            // Ensure brightness does not go below 5%
                            if (led->current_brightness < (0.03 * led->brightness))
                            {
                                led->current_brightness = (0.03 * led->brightness);
                            }
                        }
                        led->last_update = now;
                    }
                    update_led_duty(i, led->current_brightness);
                }
            }
            else
            {
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
    if (led_index >= 0 && led_index < NUM_LEDS)
    {
        led_settings[led_index].blink_sequence = (BlinkSequence){delay_ms, num_blinks, true};
        led_settings[led_index].active = true;
        led_settings[led_index].last_update = xTaskGetTickCount();
    }
}

void blinkSet(int delay_ms, int num_blinks, int led_indices[], int num_leds)
{
    for (int i = 0; i < num_leds; i++)
    {
        blink(delay_ms, num_blinks, led_indices[i]);
    }
}