#include "buttons.h"

static const char *BUTTONS_TAG = "BUTTONS";

// Prototype for button callback functions
typedef void (*button_callback_t)(void);

// Declare callback functions for each button
void button1_callback(void);
void button2_callback(void);
void button3_callback(void);
void button4_callback(void);
void button5_callback(void);

// Array of callback functions for 5 buttons
button_callback_t button_callbacks[] = {button1_callback, button2_callback, button3_callback, button4_callback, button5_callback};

void button1_long_press_callback(void);
void button2_long_press_callback(void);
void button3_long_press_callback(void);
void button4_long_press_callback(void);
void button5_long_press_callback(void);

// Array of long press callback functions for 5 buttons
button_callback_t button_long_press_callbacks[] = {button1_long_press_callback, button2_long_press_callback, button3_long_press_callback, button4_long_press_callback, button5_long_press_callback};

bool long_press_detected[5] = {false, false, false, false, false};    // Flags for long press detection
static int buttons[] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5}; // Array of button pins

void button_task(void *pvParameter)
{
    bool button_state;
    static bool last_button_state[5] = {1, 1, 1, 1, 1};
    static uint32_t button_press_timestamp[5] = {0};

    while (1)
    {
        uint32_t current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;
        for (int pinIndex = 0; pinIndex < NUM_BUTTONS; pinIndex++)
        {
            button_state = gpio_get_level(buttons[pinIndex]);

            // Debounce logic
            if (button_state != last_button_state[pinIndex])
            {
                vTaskDelay(pdMS_TO_TICKS(20)); // 20ms debounce delay
                button_state = gpio_get_level(buttons[pinIndex]);
                if (button_state != last_button_state[pinIndex])
                {
                    if (button_state == 0)
                    { // Button pressed
                        button_press_timestamp[pinIndex] = current_time;
                        long_press_detected[pinIndex] = false;
                    }
                    else
                    { // Button released
                        if (!long_press_detected[pinIndex] &&
                            (current_time - button_press_timestamp[pinIndex] < BUTTON_LONG_PRESS_DELAY_MS))
                        {
                            button_callbacks[pinIndex]();
                        }
                    }
                    last_button_state[pinIndex] = button_state;
                }
            }
            else if (button_state == 0 && !long_press_detected[pinIndex] &&
                     (current_time - button_press_timestamp[pinIndex] >= BUTTON_LONG_PRESS_DELAY_MS))
            {
                button_long_press_callbacks[pinIndex]();
                long_press_detected[pinIndex] = true;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void initialize_buttons(void)
{
    ESP_LOGI(BUTTONS_TAG, "Initializing buttons...");

    // Configure all unused pins first
    configure_unused_pins();

    // Configure button pins using gpio_config
    gpio_config_t button_conf = {
        .pin_bit_mask = (1ULL << BUTTON1) | (1ULL << BUTTON2) |
                        (1ULL << BUTTON3) | (1ULL << BUTTON4) |
                        (1ULL << BUTTON5),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    ESP_ERROR_CHECK(gpio_config(&button_conf));

    // Create button monitoring task
    BaseType_t task_created = xTaskCreate(
        button_task,
        "button_task",
        2048,
        NULL,
        10,
        NULL);

    if (task_created != pdPASS)
    {
        ESP_LOGE(BUTTONS_TAG, "Failed to create button task!");
    }
}

// Button callback implementations
void button1_callback(void)
{
    ESP_LOGI(BUTTONS_TAG, "Button 1 pressed");
    set_led_state(0, 0);
}

void button2_callback(void)
{
    ESP_LOGI(BUTTONS_TAG, "Button 2 pressed");
    set_led_state(1, 0);
}

void button3_callback(void)
{
    ESP_LOGI(BUTTONS_TAG, "Button 3 pressed");
    set_led_state(2, 0);
}

void button4_callback(void)
{
    ESP_LOGI(BUTTONS_TAG, "Button 4 pressed");
    set_led_state(3, 0);
}

void button5_callback(void)
{
    ESP_LOGI(BUTTONS_TAG, "Button 5 pressed");
    set_led_state(4, 0);
}

void button1_long_press_callback(void)
{
    ESP_LOGI(BUTTONS_TAG, "Button 1 long pressed");
    set_led_state(0, 1);
}

void button2_long_press_callback(void)
{
    ESP_LOGI(BUTTONS_TAG, "Button 2 long pressed");
    set_led_state(1, 1);
}

void button3_long_press_callback(void)
{
    ESP_LOGI(BUTTONS_TAG, "Button 3 long pressed");
    set_led_state(2, 1);
}

void button4_long_press_callback(void)
{
    ESP_LOGI(BUTTONS_TAG, "Button 4 long pressed");
    set_led_state(3, 1);
}

void button5_long_press_callback(void)
{
    ESP_LOGI(BUTTONS_TAG, "Button 5 long pressed");
    set_led_state(4, 1);
}

bool get_button_state(int pin)
{
    if (pin >= 0 && pin < NUM_BUTTONS)
    {
        return !gpio_get_level(buttons[pin]); // Inverted because of pull-up configuration
    }
    return false;
}