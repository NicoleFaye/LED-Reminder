#include "led_utils.h"

static const char *LED_TAG = "LED_UTILS";

LEDSettings led_settings[NUM_LEDS] = {0};

void initialize_ledc(void)
{
    ESP_LOGI(LED_TAG, "Initializing LEDC PWM");

    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = PWM_RESOLUTION,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = PWM_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    for (int i = 0; i < NUM_LEDS; i++)
    {
        ledc_channel_config_t ledc_channel = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel = led_settings[i].pwm_channel,
            .timer_sel = LEDC_TIMER_0,
            .intr_type = LEDC_INTR_DISABLE,
            .gpio_num = led_settings[i].pin,
            .duty = 0,
            .hpoint = 0};
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    }
}

void initialize_led(void)
{
    ESP_LOGI(LED_TAG, "Initializing LEDs");

    // Configure LED pins using gpio_config
    gpio_config_t led_conf = {
        .pin_bit_mask = (1ULL << LED1) | (1ULL << LED2) |
                        (1ULL << LED3) | (1ULL << LED4) |
                        (1ULL << LED5),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};
    ESP_ERROR_CHECK(gpio_config(&led_conf));

    // Initialize LED settings
    int led_pins[NUM_LEDS] = {LED1, LED2, LED3, LED4, LED5};

    for (int i = 0; i < NUM_LEDS; i++)
    {
        led_settings[i] = (LEDSettings){
            .pin = led_pins[i],
            .active = false,
            .function_mode = "manual",
            .display_mode = "solid",
            .brightness = 100,
            .current_brightness = 0,
            .fade_direction = 1,
            .pwm_channel = i,
            .blink_rate = 500,
            .fade_rate = 50,
            .blink_sequence = {0, 0, false},
            .last_update = 0,
            .last_on_time = 0,
            .set_time_duration = 0,
            .scheduled_off_time = 0};
        memset(led_settings[i].set_days, false, sizeof(bool) * 7);
    }

    initialize_ledc();

    // Create tasks with error checking
    BaseType_t task_created;

    task_created = xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);
    if (task_created != pdPASS)
    {
        ESP_LOGE(LED_TAG, "Failed to create LED task!");
    }

    task_created = xTaskCreate(led_set_time_task, "led_set_time_task", 4096, NULL, 5, NULL);
    if (task_created != pdPASS)
    {
        ESP_LOGE(LED_TAG, "Failed to create LED set time task!");
    }

    task_created = xTaskCreate(led_offset_task, "led_offset_task", 4096, NULL, 5, NULL);
    if (task_created != pdPASS)
    {
        ESP_LOGE(LED_TAG, "Failed to create LED offset task!");
    }

    task_created = xTaskCreate(led_fixed_interval_task, "led_fixed_interval_task", 4096, NULL, 5, NULL);
    if (task_created != pdPASS)
    {
        ESP_LOGE(LED_TAG, "Failed to create LED fixed interval task!");
    }

    task_created = xTaskCreate(led_set_days_task, "led_set_days_task", 4096, NULL, 5, NULL);
    if (task_created != pdPASS)
    {
        ESP_LOGE(LED_TAG, "Failed to create LED set days task!");
    }
}

void set_led_brightness(int led_index, int brightness)
{
    if (led_index >= 0 && led_index < NUM_LEDS)
    {
        // Clamp brightness between 1 and 100
        brightness = (brightness <= 0) ? 1 : (brightness > 100) ? 100
                                                                : brightness;
        led_settings[led_index].brightness = brightness;
        ESP_LOGD(LED_TAG, "LED %d brightness set to %d", led_index + 1, brightness);
    }
    else
    {
        ESP_LOGE(LED_TAG, "Invalid LED index: %d", led_index);
    }
}

void update_led_duty(int led_index, int brightness)
{
    if (led_index >= 0 && led_index < NUM_LEDS)
    {
        int duty = (brightness * ((1 << PWM_RESOLUTION) - 1)) / 100;
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, led_settings[led_index].pwm_channel, duty));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, led_settings[led_index].pwm_channel));
    }
}

void set_led_state(int led_index, bool state)
{
    if (led_index >= 0 && led_index < NUM_LEDS)
    {
        LEDSettings *led = &led_settings[led_index];
        led->active = state;

        if (!state && strcmp(led->function_mode, "offset") == 0)
        {
            led->last_update = xTaskGetTickCount();
        }

        update_led_duty(led_index, state ? led->brightness : 0);
        ESP_LOGI(LED_TAG, "LED %d state set to %s", led_index + 1, state ? "ON" : "OFF");
    }
    else
    {
        ESP_LOGE(LED_TAG, "Invalid LED index: %d", led_index);
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

    ESP_LOGI(LED_TAG, "LED task started");

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
                    update_led_duty(i, led->active ? led->brightness : 0);
                }
            }
            else if (led->active)
            {
                if (strcmp(led->display_mode, "solid") == 0)
                {
                    update_led_duty(i, led->brightness);
                }
                else if (strcmp(led->display_mode, "blink") == 0)
                {
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

                    if (!synced && (now - led->last_update) >= pdMS_TO_TICKS(led->blink_rate))
                    {
                        led->blinking = !led->blinking;
                        led->last_update = now;
                    }
                    update_led_duty(i, led->blinking ? led->brightness : 0);
                }
                else if (strcmp(led->display_mode, "fade") == 0)
                {
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

                    if (!synced && (now - led->last_update) >= pdMS_TO_TICKS(led->fade_rate))
                    {
                        led->current_brightness += led->fade_direction;

                        // Handle brightness limits and direction changes
                        if (led->current_brightness >= led->brightness ||
                            led->current_brightness <= (int)(0.03 * led->brightness))
                        {
                            led->fade_direction *= -1;
                            led->current_brightness =
                                (led->current_brightness >= led->brightness) ? led->brightness : (int)(0.03 * led->brightness);
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
        led_settings[led_index].blink_sequence = (BlinkSequence){
            .delay_ms = delay_ms,
            .num_blinks = num_blinks,
            .active = true};
        led_settings[led_index].active = true;
        led_settings[led_index].last_update = xTaskGetTickCount();
        ESP_LOGI(LED_TAG, "Started blink sequence for LED %d: %d ms delay, %d blinks",
                 led_index + 1, delay_ms, num_blinks);
    }
    else
    {
        ESP_LOGE(LED_TAG, "Invalid LED index for blink: %d", led_index);
    }
}

void blinkSet(int delay_ms, int num_blinks, int led_indices[], int num_leds)
{
    ESP_LOGI(LED_TAG, "Starting blink sequence for multiple LEDs");

    for (int i = 0; i < num_leds; i++)
    {
        if (led_indices[i] >= 0 && led_indices[i] < NUM_LEDS)
        {
            blink(delay_ms, num_blinks, led_indices[i]);
        }
        else
        {
            ESP_LOGE(LED_TAG, "Invalid LED index in blinkSet: %d", led_indices[i]);
        }
    }
}

void led_offset_task(void *pvParameters)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t frequency = pdMS_TO_TICKS(100);

    ESP_LOGI(LED_TAG, "LED offset task started");

    while (1)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            LEDSettings *led = &led_settings[i];

            if (strcmp(led->function_mode, "offset") == 0 && !led->active)
            {
                if ((xTaskGetTickCount() - led->last_update) >= pdMS_TO_TICKS(led->offset_seconds * 1000))
                {
                    set_led_state(i, true);
                    ESP_LOGI(LED_TAG, "LED %d activated by offset timer", i + 1);
                }
            }
        }
        vTaskDelayUntil(&last_wake_time, frequency);
    }
}

void led_fixed_interval_task(void *pvParameters)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t frequency = pdMS_TO_TICKS(1000);

    ESP_LOGI(LED_TAG, "LED fixed interval task started");

    while (1)
    {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);

        for (int i = 0; i < NUM_LEDS; i++)
        {
            LEDSettings *led = &led_settings[i];

            if (strcmp(led->function_mode, "fixed_interval") == 0)
            {
                int seconds_since_midnight = timeinfo.tm_hour * 3600 +
                                             timeinfo.tm_min * 60 +
                                             timeinfo.tm_sec;
                int ref_seconds = led->set_time_hours * 3600 +
                                  led->set_time_minutes * 60;
                int seconds_since_ref = (seconds_since_midnight - ref_seconds + 86400) % 86400;

                if (seconds_since_ref % led->fixed_interval_seconds == 0)
                {
                    set_led_state(i, true);
                    ESP_LOGI(LED_TAG, "LED %d activated by fixed interval", i + 1);
                }
            }
        }
        vTaskDelayUntil(&last_wake_time, frequency);
    }
}

void led_set_time_task(void *pvParameters)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t frequency = pdMS_TO_TICKS(1000);

    ESP_LOGI(LED_TAG, "LED set time task started");

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
                // Activation check
                if (timeinfo.tm_hour == led->set_time_hours &&
                    timeinfo.tm_min == led->set_time_minutes &&
                    timeinfo.tm_sec == 0)
                {

                    if (difftime(now, led->last_on_time) >= (led->set_time_days * 24 * 60 * 60))
                    {
                        set_led_state(i, true);
                        led->last_on_time = now;
                        ESP_LOGI(LED_TAG, "LED %d activated by set time", i + 1);

                        if (led->set_time_duration > 0)
                        {
                            led->scheduled_off_time = now + led->set_time_duration;
                            ESP_LOGI(LED_TAG, "LED %d scheduled to turn off in %d seconds",
                                     i + 1, led->set_time_duration);
                        }
                    }
                }

                // Deactivation check
                if (led->set_time_duration > 0 &&
                    led->scheduled_off_time > 0 &&
                    now >= led->scheduled_off_time)
                {
                    set_led_state(i, false);
                    led->scheduled_off_time = 0;
                    ESP_LOGI(LED_TAG, "LED %d deactivated by duration timer", i + 1);
                }
            }
        }
        vTaskDelayUntil(&last_wake_time, frequency);
    }
}
void led_set_days_task(void *pvParameters)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t frequency = pdMS_TO_TICKS(1000); // Check every second

    ESP_LOGI(LED_TAG, "LED set days task started");

    while (1)
    {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);

        for (int i = 0; i < NUM_LEDS; i++)
        {
            LEDSettings *led = &led_settings[i];

            if (strcmp(led->function_mode, "set_days") == 0)
            {
                // Check if today is a set day and it's the correct time
                if (led->set_days[timeinfo.tm_wday] &&
                    timeinfo.tm_hour == led->set_time_hours &&
                    timeinfo.tm_min == led->set_time_minutes &&
                    timeinfo.tm_sec == 0)
                {

                    set_led_state(i, true);
                    ESP_LOGI(LED_TAG, "LED %d activated by set days schedule", i + 1);

                    if (led->set_time_duration > 0)
                    {
                        led->scheduled_off_time = now + led->set_time_duration;
                        ESP_LOGI(LED_TAG, "LED %d scheduled to turn off in %d seconds",
                                 i + 1, led->set_time_duration);
                    }
                }

                // Check for scheduled turn off
                if (led->set_time_duration > 0 &&
                    led->scheduled_off_time > 0 &&
                    now >= led->scheduled_off_time)
                {
                    set_led_state(i, false);
                    led->scheduled_off_time = 0;
                    ESP_LOGI(LED_TAG, "LED %d deactivated by duration timer", i + 1);
                }
            }
        }
        vTaskDelayUntil(&last_wake_time, frequency);
    }
}