#include "blink.h"

void initialize_led(void)
{
    for(int i = 6; i < 12; i++){
        gpio_reset_pin(i);
        gpio_set_direction(i, GPIO_MODE_OUTPUT);
    }
}

void blink(int delay_ms, int num_blinks, int pin)
{
    for (int i = 0; i < num_blinks; i++)
    {
        gpio_set_level(pin, 1);
        vTaskDelay(delay_ms / portTICK_PERIOD_MS);
        gpio_set_level(pin, 0);
        vTaskDelay(delay_ms / portTICK_PERIOD_MS);
    }
}

void blinkSet(int delay_ms, int num_blinks, int pins[], int num_pins){
    for (int i = 0; i < num_blinks; i++)
    {
        for (int j = 0; j < num_pins; j++)
        {
            gpio_set_level(pins[j], 1);
        }
        vTaskDelay(delay_ms / portTICK_PERIOD_MS);
        for (int j = 0; j < num_pins; j++)
        {
            gpio_set_level(pins[j], 0);
        }
        vTaskDelay(delay_ms / portTICK_PERIOD_MS);
    }
}
