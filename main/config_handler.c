#include "config_handler.h"

static void apply_wifi_config(const KeyValuePair *config, int count)
{
    char ssid[64] = "";
    char password[64] = "";

    for (int i = 0; i < count; i++)
    {
        if (strcmp(config[i].key, "ssid") == 0)
        {
            strncpy(ssid, config[i].value, sizeof(ssid) - 1);
        }
        else if (strcmp(config[i].key, "password") == 0)
        {
            strncpy(password, config[i].value, sizeof(password) - 1);
        }
    }

    if (strlen(ssid) > 0 && strlen(password) > 0)
    {
        ESP_LOGI(TAG, "Configuring WiFi with SSID: %s", ssid);
        // Call function to set up WiFi
        // wifi_setup(ssid, password);
    }
}

static void apply_led_config(const KeyValuePair *config, int count)
{
    for (int i = 0; i < count; i++)
    {
        // Extract the LED number from the key
        int led_num;
        if (sscanf(config[i].key, "led%d_", &led_num) != 1) {
            continue; // Skip if the key doesn't start with "ledX_"
        }

        // Find the corresponding LED in led_settings based on pin number
        int led_index = -1;
        for (int j = 0; j < NUM_LEDS; j++) {
            if (led_settings[j].pin == led_num) {
                led_index = j;
                break;
            }
        }

        if (led_index == -1) {
            ESP_LOGW(TAG, "No LED found for pin %d", led_num);
            continue; // Skip if no matching LED found
        }

        // Extract the setting name
        const char *setting = strchr(config[i].key, '_');
        if (setting == NULL) {
            continue; // Skip if there's no underscore in the key
        }
        setting++; // Move past the underscore

        if (strcmp(setting, "function_mode") == 0)
        {
            strncpy(led_settings[led_index].function_mode, config[i].value, sizeof(led_settings[led_index].function_mode) - 1);
            ESP_LOGI(TAG, "Setting LED %d function mode to %s", led_num, led_settings[led_index].function_mode);
        }
        else if (strcmp(setting, "offset_seconds") == 0)
        {
            led_settings[led_index].offset_seconds = atoi(config[i].value);
            ESP_LOGI(TAG, "Setting LED %d offset seconds to %d", led_num, led_settings[led_index].offset_seconds);
        }
        else if (strcmp(setting, "set_time_days") == 0)
        {
            led_settings[led_index].set_time_days = atoi(config[i].value);
            ESP_LOGI(TAG, "Setting LED %d set time days to %d", led_num, led_settings[led_index].set_time_days);
        }
        else if (strcmp(setting, "fixed_interval_seconds") == 0)
        {
            led_settings[led_index].fixed_interval_seconds = atoi(config[i].value);
            ESP_LOGI(TAG, "Setting LED %d fixed interval to %d", led_num, led_settings[led_index].fixed_interval_seconds);
        }
        else if (strcmp(setting, "display_mode") == 0)
        {
            strncpy(led_settings[led_index].display_mode, config[i].value, sizeof(led_settings[led_index].display_mode) - 1);
            ESP_LOGI(TAG, "Setting LED %d display mode to %s", led_num, led_settings[led_index].display_mode);
        }
        else if (strcmp(setting, "blink_rate") == 0)
        {
            led_settings[led_index].blink_rate = atoi(config[i].value);
            ESP_LOGI(TAG, "Setting LED %d blink rate to %d", led_num, led_settings[led_index].blink_rate);
        }
        else if (strcmp(setting, "fade_rate") == 0)
        {
            led_settings[led_index].fade_rate = atoi(config[i].value);
            ESP_LOGI(TAG, "Setting LED %d fade rate to %d", led_num, led_settings[led_index].fade_rate);
        }
        else if (strcmp(setting, "brightness") == 0)
        {
            int brightness = atoi(config[i].value);
            led_settings[led_index].brightness = brightness;
            set_led_brightness(led_index, brightness);
            ESP_LOGI(TAG, "Setting LED %d brightness to %d", led_num, brightness);
        }
    }
}

static void apply_time_config(const KeyValuePair *config, int count)
{
    char timezone[64] = "";

    for (int i = 0; i < count; i++)
    {
        if (strcmp(config[i].key, "timezone") == 0)
        {
            strncpy(timezone, config[i].value, sizeof(timezone) - 1);
            break;
        }
    }

    if (strlen(timezone) > 0)
    {
        ESP_LOGI(TAG, "Setting timezone to: %s", timezone);
        // Call function to set timezone
        // set_timezone(timezone);
    }
}

void apply_configuration(void)
{
    int kv_count;
    KeyValuePair *config = parse_configuration(BASE_PATH "/settings.txt", &kv_count);
    if (config == NULL)
    {
        ESP_LOGE(TAG, "Failed to parse configuration");
        return;
    }

    apply_wifi_config(config, kv_count);
    apply_led_config(config, kv_count);
    apply_time_config(config, kv_count);

    free(config);
}
