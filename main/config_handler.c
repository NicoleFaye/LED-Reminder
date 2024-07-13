#include "config_handler.h"


static void apply_wifi_config(const KeyValuePair* config, int count) {
    char ssid[64] = "";
    char password[64] = "";

    for (int i = 0; i < count; i++) {
        if (strcmp(config[i].key, "ssid") == 0) {
            strncpy(ssid, config[i].value, sizeof(ssid) - 1);
        } else if (strcmp(config[i].key, "password") == 0) {
            strncpy(password, config[i].value, sizeof(password) - 1);
        }
    }

    if (strlen(ssid) > 0 && strlen(password) > 0) {
        ESP_LOGI(TAG, "Configuring WiFi with SSID: %s", ssid);
        // Call function to set up WiFi
        // wifi_setup(ssid, password);
    }
}

static void apply_led_config(const KeyValuePair* config, int count) {
    LEDSettings led_settings[5] = {0};

    for (int i = 0; i < count; i++) {
        for (int led = 1; led <= 5; led++) {
            char led_prefix[10];
            snprintf(led_prefix, sizeof(led_prefix), "led%d_", led);

            if (strncmp(config[i].key, led_prefix, strlen(led_prefix)) == 0) {
                const char* setting = config[i].key + strlen(led_prefix);
                
                if (strcmp(setting, "function_mode") == 0) {
                    strncpy(led_settings[led-1].function_mode, config[i].value, sizeof(led_settings[led-1].function_mode) - 1);
                } else if (strcmp(setting, "offset_seconds") == 0) {
                    led_settings[led-1].offset_seconds = atoi(config[i].value);
                } else if (strcmp(setting, "set_time_days") == 0) {
                    led_settings[led-1].set_time_days = atoi(config[i].value);
                } else if (strcmp(setting, "fixed_interval_seconds") == 0) {
                    led_settings[led-1].fixed_interval_seconds = atoi(config[i].value);
                } else if (strcmp(setting, "display_mode") == 0) {
                    strncpy(led_settings[led-1].display_mode, config[i].value, sizeof(led_settings[led-1].display_mode) - 1);
                } else if (strcmp(setting, "blink_rate") == 0) {
                    led_settings[led-1].blink_rate = atoi(config[i].value);
                } else if (strcmp(setting, "fade_rate") == 0) {
                    led_settings[led-1].fade_rate = atoi(config[i].value);
                } else if (strcmp(setting, "brightness") == 0) {
                    led_settings[led-1].brightness = atoi(config[i].value);
                }
            }
        }
    }

    for (int led = 0; led < 5; led++) {
        ESP_LOGI(TAG, "Configuring LED %d", led + 1);
        ESP_LOGI(TAG, "  Function mode: %s", led_settings[led].function_mode);
        ESP_LOGI(TAG, "  Display mode: %s", led_settings[led].display_mode);
        ESP_LOGI(TAG, "  Brightness: %d", led_settings[led].brightness);

        // Apply LED settings
        // configure_led(led + 1, &led_settings[led]);
    }
}

static void apply_time_config(const KeyValuePair* config, int count) {
    char timezone[64] = "";

    for (int i = 0; i < count; i++) {
        if (strcmp(config[i].key, "timezone") == 0) {
            strncpy(timezone, config[i].value, sizeof(timezone) - 1);
            break;
        }
    }

    if (strlen(timezone) > 0) {
        ESP_LOGI(TAG, "Setting timezone to: %s", timezone);
        // Call function to set timezone
        // set_timezone(timezone);
    }
}

void apply_configuration(void) {
    int kv_count;
    KeyValuePair* config = parse_configuration(BASE_PATH "/settings.txt", &kv_count);
    if (config == NULL) {
        ESP_LOGE(TAG, "Failed to parse configuration");
        return;
    }

    apply_wifi_config(config, kv_count);
    apply_led_config(config, kv_count);
    apply_time_config(config, kv_count);

    free(config);
}
