#include "gpio_config.h"

static const char *GPIO_TAG = "GPIO_CONFIG";

bool is_special_pin(int pin)
{
    // Check if pin is a strapping pin
    if (pin == STRAPPING_PIN_0 || pin == STRAPPING_PIN_3 ||
        pin == STRAPPING_PIN_45 || pin == STRAPPING_PIN_46)
    {
        return true;
    }

    // Check if pin is in flash range
    if (pin >= FLASH_START_PIN && pin <= FLASH_END_PIN)
    {
        return true;
    }

    // Check if pin is in PSRAM range
    if (pin >= PSRAM_START_PIN && pin <= PSRAM_END_PIN)
    {
        return true;
    }

    // Check if pin is in JTAG/USB range
    if (pin == USB_DM_PIN || pin == USB_DP_PIN ||
        (pin >= JTAG_START_PIN && pin <= JTAG_END_PIN))
    {
        return true;
    }

    // Check for non-existent pins
    if (pin >= 22 && pin <= 25) // These pins are not exposed
    {
        return true;
    }

    return false;
}

void configure_unused_pins(void)
{
    // Create bitmasks for used pins
    uint64_t led_pin_mask = (1ULL << LED1) | (1ULL << LED2) |
                            (1ULL << LED3) | (1ULL << LED4) |
                            (1ULL << LED5);

    uint64_t button_pin_mask = (1ULL << BUTTON1) | (1ULL << BUTTON2) |
                               (1ULL << BUTTON3) | (1ULL << BUTTON4) |
                               (1ULL << BUTTON5);

    // Configure each GPIO pin that's not used and not special
    for (int pin = 0; pin < GPIO_NUM_MAX; pin++)
    {
        // Skip if pin is used by LED or button
        if ((led_pin_mask & (1ULL << pin)) || (button_pin_mask & (1ULL << pin)))
        {
            continue;
        }

        // Skip if pin is special
        if (is_special_pin(pin))
        {
            continue;
        }

        // Configure as input with pull-down
        gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << pin),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_ENABLE,
            .intr_type = GPIO_INTR_DISABLE};

        ESP_LOGI(GPIO_TAG, "Configuring GPIO%d as pulled-down input", pin);
        gpio_config(&io_conf);
    }
}