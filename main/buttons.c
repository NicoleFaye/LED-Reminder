#include <buttons.h>

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


static bool long_press_detected[5] = {false, false, false, false, false}; // Flags for long press detection

static int buttons[] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5}; // Array of button pins

void button_task(void *pvParameter) {
    bool button_state;
    static bool last_button_state[5] = {1, 1, 1, 1, 1};
    static uint32_t button_press_timestamp[5] = {0}; // Timestamps for when buttons were pressed

    while (1) {
        uint32_t current_time = xTaskGetTickCount() * portTICK_PERIOD_MS; // Get current time in ms
        for (int pinIndex = 0; pinIndex <= NUM_BUTTONS; pinIndex++){
            button_state = gpio_get_level(buttons[pinIndex]);
            if (button_state == 0 && last_button_state[pinIndex] == 1) {
                // Button press detected, record timestamp
                button_press_timestamp[pinIndex] = current_time;
                long_press_detected[pinIndex] = false; // Reset long press flag
            } else if (button_state == 0 && !long_press_detected[pinIndex] && (current_time - button_press_timestamp[pinIndex] >= BUTTON_LONG_PRESS_DELAY_MS)) {
                // Button held for 3 seconds, call long press callback and set flag to avoid short press callback
                button_long_press_callbacks[pinIndex]();
                long_press_detected[pinIndex] = true;
            } else if (button_state == 1 && last_button_state[pinIndex] == 0) {
                if (!long_press_detected[pinIndex] && (current_time - button_press_timestamp[pinIndex] < BUTTON_LONG_PRESS_DELAY_MS)) {
                    // Button released before x seconds and no long press detected, call short press callback
                    button_callbacks[pinIndex]();
                }
                // Reset the long press flag here if you want to allow a long press on the next button down event
                long_press_detected[pinIndex] = false;
            }
            last_button_state[pinIndex] = button_state;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// Initialize buttons, callback implementations for short and long press...

void button1_long_press_callback(void) {
    ESP_LOGI("BUTTON", "Button 1 long pressed");
    set_led_state(0, 1);
}

void button2_long_press_callback(void) {
    ESP_LOGI("BUTTON", "Button 2 long pressed");
    set_led_state(1, 1);
}

void button3_long_press_callback(void) {
    ESP_LOGI("BUTTON", "Button 3 long pressed");
    set_led_state(2, 1);
}

void button4_long_press_callback(void) {
    ESP_LOGI("BUTTON", "Button 4 long pressed");
    set_led_state(3, 1);
}

void button5_long_press_callback(void) {
    ESP_LOGI("BUTTON", "Button 5 long pressed");
    set_led_state(4, 1);
}

void initialize_buttons(void) {
    for (int i = BUTTON1; i <= BUTTON5; i++) { // Adjusted for 5 buttons
        gpio_reset_pin(i);
        gpio_set_direction(i, GPIO_MODE_INPUT);
        gpio_set_pull_mode(i, GPIO_PULLUP_ONLY);
    }

    // TODO handle custom strapping on startup


    // Create a task to monitor button presses
    xTaskCreate(&button_task, "button_task", 2048, NULL, 10, NULL);
}

// Example callback implementations
void button1_callback(void) {
    ESP_LOGI("BUTTON", "Button 1 pressed");
    set_led_state(0, 0);
}

void button2_callback(void) {
    ESP_LOGI("BUTTON", "Button 2 pressed");
    set_led_state(1, 0);
}

void button3_callback(void) {
    ESP_LOGI("BUTTON", "Button 3 pressed");
    set_led_state(2, 0);
}

void button4_callback(void) {
    ESP_LOGI("BUTTON", "Button 4 pressed");
    set_led_state(3, 0);
}

void button5_callback(void) { 
    ESP_LOGI("BUTTON", "Button 5 pressed");
    set_led_state(4, 0);
}