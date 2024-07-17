#include "config_handler.h"

char ssid[64] = "";
char password[64] = "";
const char *posix_tz = NULL;

static void apply_wifi_config(const KeyValuePair *config, int count)
{

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
}
void connect_to_wifi(void)
{
    wifi_connect(ssid, password);
}

static void apply_led_config(const KeyValuePair *config, int count)
{
    for (int i = 0; i < count; i++)
    {
        // Extract the LED number from the key
        int led_num;
        if (sscanf(config[i].key, "led%d_", &led_num) != 1)
        {
            continue; // Skip if the key doesn't start with "ledX_"
        }

        // Find the corresponding LED in led_settings based on pin number
        int led_index = -1;
        for (int j = 0; j < NUM_LEDS; j++)
        {
            if (led_settings[j].pin == led_num)
            {
                led_index = j;
                break;
            }
        }

        if (led_index == -1)
        {
            ESP_LOGW(TAG, "No LED found for pin %d", led_num);
            continue; // Skip if no matching LED found
        }

        // Extract the setting name
        const char *setting = strchr(config[i].key, '_');
        if (setting == NULL)
        {
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
        else if (strcmp(setting, "set_days") == 0)
        {
            // Initialize all days to false
            for (int j = 0; j < 7; j++)
            {
                led_settings[led_index].set_days[j] = false;
            }

            // Create a mutable copy of the string
            char value_copy[MAX_LINE_LENGTH];
            strncpy(value_copy, config[i].value, sizeof(value_copy) - 1);
            value_copy[sizeof(value_copy) - 1] = '\0'; // Ensure null-termination

            char *day_str = strtok(value_copy, ",");
            while (day_str != NULL)
            {
                if (strcmp(day_str, "sunday") == 0)
                    led_settings[led_index].set_days[0] = true;
                else if (strcmp(day_str, "monday") == 0)
                    led_settings[led_index].set_days[1] = true;
                else if (strcmp(day_str, "tuesday") == 0)
                    led_settings[led_index].set_days[2] = true;
                else if (strcmp(day_str, "wednesday") == 0)
                    led_settings[led_index].set_days[3] = true;
                else if (strcmp(day_str, "thursday") == 0)
                    led_settings[led_index].set_days[4] = true;
                else if (strcmp(day_str, "friday") == 0)
                    led_settings[led_index].set_days[5] = true;
                else if (strcmp(day_str, "saturday") == 0)
                    led_settings[led_index].set_days[6] = true;
                day_str = strtok(NULL, ",");
            }
            ESP_LOGI(TAG, "Setting LED %d set days", led_num);
            for (int j = 0; j < 7; j++)
            {
                ESP_LOGI(TAG, "Day %d: %d", j, led_settings[led_index].set_days[j]);
            }
        }
        else if (strcmp(setting, "set_time_days") == 0)
        {
            led_settings[led_index].set_time_days = atoi(config[i].value);
            ESP_LOGI(TAG, "Setting LED %d set time days to %d", led_num, led_settings[led_index].set_time_days);
        }
        else if (strcmp(setting, "set_time") == 0)
        {
            int hours, minutes;
            if (sscanf(config[i].value, "%d:%d", &hours, &minutes) == 2)
            {
                led_settings[led_index].set_time_hours = hours;
                led_settings[led_index].set_time_minutes = minutes;
                ESP_LOGI(TAG, "Setting LED %d set time to %02d:%02d", led_num, hours, minutes);
            }
            else
            {
                ESP_LOGE(TAG, "Invalid set_time format for LED %d: %s", led_num, config[i].value);
            }
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
        // const char *posix_tz = get_timezone(timezone);
        posix_tz = get_timezone(timezone);
        if (posix_tz != NULL)
        {
            ESP_LOGI(TAG, "Setting timezone to: %s (POSIX: %s)", timezone, posix_tz);
            // Call function to set timezone
            // initialize_sntp(posix_tz);
        }
        else
        {
            ESP_LOGE(TAG, "Invalid timezone: %s", timezone);
        }
    }
    else
    {
        ESP_LOGW(TAG, "No timezone specified in configuration");
    }
}

void sync_time(void)
{
    initialize_sntp(posix_tz);
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
