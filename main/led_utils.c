#include "led_utils.h"

// Global array of LED settings
LEDSettings led_settings[NUM_LEDS] = {0};

void initialize_led(void)
{
    for(int i = LED1; i <= LED5; i++){
        gpio_reset_pin(i);
        gpio_set_direction(i, GPIO_MODE_OUTPUT);
        gpio_set_level(i, 0);  // Initialize all LEDs to off
    }

    // Create the LED management task
    xTaskCreate(led_task, "led_task", 2048, NULL, 5, NULL);
}

void set_led_state(int led_pin, bool state)
{
    if (led_pin >= LED1 && led_pin <= LED5) {
        led_settings[led_pin - LED1].blink_sequence.active = false;
        gpio_set_level(led_pin, state);
    }
}

void led_task(void *pvParameters)
{
    TickType_t last_wake_time;
    const TickType_t frequency = pdMS_TO_TICKS(50);  // Update every 50ms

    last_wake_time = xTaskGetTickCount();

    while (1) {
        for (int i = 0; i < NUM_LEDS; i++) {
            if (led_settings[i].blink_sequence.active) {
                static int blink_count[NUM_LEDS] = {0};
                static bool led_on[NUM_LEDS] = {false};
                static TickType_t last_toggle[NUM_LEDS] = {0};

                TickType_t now = xTaskGetTickCount();
                if ((now - last_toggle[i]) >= pdMS_TO_TICKS(led_settings[i].blink_sequence.delay_ms)) {
                    led_on[i] = !led_on[i];
                    gpio_set_level(i + LED1, led_on[i]);
                    last_toggle[i] = now;

                    if (!led_on[i]) {
                        blink_count[i]++;
                        if (blink_count[i] >= led_settings[i].blink_sequence.num_blinks) {
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

void blink(int delay_ms, int num_blinks, int pin)
{
    if (pin >= LED1 && pin <= LED5) {
        led_settings[pin - LED1].blink_sequence = (BlinkSequence){delay_ms, num_blinks, true};
    }
}

void blinkSet(int delay_ms, int num_blinks, int pins[], int num_pins)
{
    for (int i = 0; i < num_pins; i++) {
        if (pins[i] >= LED1 && pins[i] <= LED5) {
            led_settings[pins[i] - LED1].blink_sequence = (BlinkSequence){delay_ms, num_blinks, true};
        }
    }
}