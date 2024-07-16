#include "parse.h"

void trim_whitespace(char *str)
{
    char *end;

    while (*str == ' ' || *str == '\t' || *str == '\n')
        str++;

    if (*str == 0)
        return;

    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n'))
        end--;

    end[1] = '\0';
}

int parse_line(char *line, KeyValuePair *kv)
{
    char *delimiter = strchr(line, '=');
    if (delimiter == NULL)
    {
        return 0; // Invalid line format
    }

    *delimiter = '\0';
    strncpy(kv->key, line, MAX_LINE_LENGTH);
    strncpy(kv->value, delimiter + 1, MAX_LINE_LENGTH);

    trim_whitespace(kv->key);
    trim_whitespace(kv->value);

    return 1;
}

int validate_led_setting(KeyValuePair kv, int led_num)
{
    if (strncmp(kv.key, "led", 3) != 0)
    {
        return 1; // Not an LED setting
    }

    char led_id[3];
    snprintf(led_id, 3, "%d", led_num);

    if (strstr(kv.key, led_id) == NULL)
    {
        return 1; // Not for this LED
    }

    if (strstr(kv.key, "function_mode") != NULL)
    {
        if (strcmp(kv.value, "manual") != 0 && strcmp(kv.value, "offset") != 0 &&
            strcmp(kv.value, "set_time") != 0 && strcmp(kv.value, "fixed_interval") != 0)
        {
            return 0; // Invalid function mode
        }
    }
    else if (strstr(kv.key, "display_mode") != NULL)
    {
        if (strcmp(kv.value, "solid") != 0 && strcmp(kv.value, "blink") != 0 &&
            strcmp(kv.value, "fade") != 0)
        {
            return 0; // Invalid display mode
        }
    }
    else if (strstr(kv.key, "brightness") != NULL)
    {
        int brightness = atoi(kv.value);
        if (brightness < 0 || brightness > 100)
        {
            return 0; // Invalid brightness
        }
    }
    else if (strstr(kv.key, "set_time_days") != NULL)
    {
        int days = atoi(kv.value);
        if (days < 0 || days > 365 * 10)
        {
            return 0; // Invalid days
        }
    }
    else if (strstr(kv.key, "set_time") != NULL)
    {
        // Validate the time format (HH:MM)
        int hours, minutes;
        if (sscanf(kv.value, "%d:%d", &hours, &minutes) != 2 ||
            hours < 0 || hours > 23 || minutes < 0 || minutes > 59)
        {
            return 0; // Invalid time format
        }
    }
    else
    {
        // Other LED settings can be validated as needed
    }

    return 1;
}

int check_configuration(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        ESP_LOGE(TAG, "Failed to open file: %s", filename);
        return 69;
    }

    char line[MAX_LINE_LENGTH];
    KeyValuePair kv;

    int valid_timezone = 0;
    int valid_ssid = 0;
    int valid_password = 0;
    int valid_leds[NUM_LEDS] = {0};

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '#' || line[0] == '\n')
        {
            continue;
        }

        if (!parse_line(line, &kv))
        {
            continue;
        }

        if (strcmp(kv.key, "timezone") == 0)
        {
            if (get_timezone(kv.value) != NULL)
            {
                valid_timezone = 1;
            }
            else
            {
                ESP_LOGE(TAG, "Invalid timezone: %s", kv.value);
            }
        }
        else if (strcmp(kv.key, "ssid") == 0)
        {
            valid_ssid = 1;
        }
        else if (strcmp(kv.key, "password") == 0)
        {
            valid_password = 1;
        }
        else
        {
            for (int i = 1; i <= NUM_LEDS; ++i)
            {
                if (!validate_led_setting(kv, i))
                {
                    ESP_LOGE(TAG, "Invalid setting for LED %d: %s = %s", i, kv.key, kv.value);
                    valid_leds[i - 1] = 0;
                }
                else
                {
                    valid_leds[i - 1] = 1;
                }
            }
        }
    }

    fclose(file);

    if (!valid_timezone)
    {
        ESP_LOGE(TAG, "Invalid or missing timezone setting.");
        return 1;
    }

    if (!valid_ssid)
    {
        ESP_LOGE(TAG, "Invalid or missing Wi-Fi SSID setting.");
        return 2;
    }

    if (!valid_password)
    {
        ESP_LOGE(TAG, "Invalid or missing Wi-Fi password setting.");
        return 3;
    }

    for (int i = 0; i < NUM_LEDS; ++i)
    {
        if (!valid_leds[i])
        {
            ESP_LOGE(TAG, "Invalid settings for LED %d.", i + 1);
            return 4 + i;
        }
    }

    ESP_LOGI(TAG, "Configuration file is valid.");
    return 0;
}

KeyValuePair *parse_configuration(const char *filename, int *length)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        ESP_LOGE(TAG, "Failed to open file: %s", filename);
        *length = 0;
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    KeyValuePair *kv_array = NULL;
    int kv_count = 0;
    int kv_capacity = 10; // Initial capacity

    kv_array = malloc(kv_capacity * sizeof(KeyValuePair));
    if (!kv_array)
    {
        ESP_LOGE(TAG, "Memory allocation failed");
        fclose(file);
        *length = 0;
        return NULL;
    }

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '#' || line[0] == '\n')
        {
            continue;
        }

        if (kv_count >= kv_capacity)
        {
            kv_capacity *= 2;
            KeyValuePair *temp = realloc(kv_array, kv_capacity * sizeof(KeyValuePair));
            if (!temp)
            {
                ESP_LOGE(TAG, "Memory reallocation failed");
                free(kv_array);
                fclose(file);
                *length = 0;
                return NULL;
            }
            kv_array = temp;
        }

        if (parse_line(line, &kv_array[kv_count]))
        {
            kv_count++;
        }
    }

    fclose(file);
    *length = kv_count;
    ESP_LOGI(TAG, "Parsed %d key-value pairs from configuration file", kv_count);
    return kv_array;
}

void parse_and_handle_settings(void) {
    int kv_count;
    KeyValuePair *config = parse_configuration(BASE_PATH "/settings.txt", &kv_count);
    if (config == NULL) {
        ESP_LOGE(TAG, "Failed to parse configuration");
        return;
    }

    char timezone[64] = "";
    char ssid[64] = "";
    char password[64] = "";
    LEDSettings led_settings[5] = {0};

    for (int i = 0; i < kv_count; i++) {
        ESP_LOGI(TAG, "%s = %s", config[i].key, config[i].value);

        if (strcmp(config[i].key, "timezone") == 0) {
            strncpy(timezone, config[i].value, sizeof(timezone) - 1);
        } else if (strcmp(config[i].key, "ssid") == 0) {
            strncpy(ssid, config[i].value, sizeof(ssid) - 1);
        } else if (strcmp(config[i].key, "password") == 0) {
            strncpy(password, config[i].value, sizeof(password) - 1);
        } else {
            // Handle LED settings
            for (int led = 1; led <= 5; led++) {
                char led_prefix[10];
                snprintf(led_prefix, sizeof(led_prefix), "led%d_", led);

                if (strncmp(config[i].key, led_prefix, strlen(led_prefix)) == 0) {
                    char* setting = config[i].key + strlen(led_prefix);
                    
                    if (strcmp(setting, "function_mode") == 0) {
                        strncpy(led_settings[led-1].function_mode, config[i].value, sizeof(led_settings[led-1].function_mode) - 1);
                    } else if (strcmp(setting, "offset_seconds") == 0) {
                        led_settings[led-1].offset_seconds = atoi(config[i].value);
                    } else if (strcmp(setting, "set_time_days") == 0) {
                        led_settings[led-1].set_time_days = atoi(config[i].value);
                    } else if (strcmp(setting, "set_time") == 0) {
                        sscanf(config[i].value, "%d:%d", &led_settings[led-1].set_time_hours, &led_settings[led-1].set_time_minutes);
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
    }

    // Handle parsed settings
    if (strlen(timezone) > 0) {
        ESP_LOGI(TAG, "Setting timezone to: %s", timezone);
        // Call function to set timezone
        // set_timezone(timezone);
    }

    if (strlen(ssid) > 0 && strlen(password) > 0) {
        ESP_LOGI(TAG, "Connecting to WiFi SSID: %s", ssid);
        // Call function to connect to WiFi
        // connect_wifi(ssid, password);
    }

    for (int led = 0; led < 5; led++) {
        ESP_LOGI(TAG, "Configuring LED %d", led + 1);
        ESP_LOGI(TAG, "  Function mode: %s", led_settings[led].function_mode);
        ESP_LOGI(TAG, "  Display mode: %s", led_settings[led].display_mode);
        ESP_LOGI(TAG, "  Brightness: %d", led_settings[led].brightness);

        if (strcmp(led_settings[led].function_mode, "manual") == 0) {
            // Handle manual mode
        } else if (strcmp(led_settings[led].function_mode, "offset") == 0) {
            ESP_LOGI(TAG, "  Offset seconds: %d", led_settings[led].offset_seconds);
            // Handle offset mode
        } else if (strcmp(led_settings[led].function_mode, "set_time") == 0) {
            ESP_LOGI(TAG, "  Set time days: %d", led_settings[led].set_time_days);
            // Handle set time mode
        } else if (strcmp(led_settings[led].function_mode, "fixed_interval") == 0) {
            ESP_LOGI(TAG, "  Fixed interval seconds: %d", led_settings[led].fixed_interval_seconds);
            // Handle fixed interval mode
        }

        if (strcmp(led_settings[led].display_mode, "solid") == 0) {
            // Handle solid mode
        } else if (strcmp(led_settings[led].display_mode, "blink") == 0) {
            ESP_LOGI(TAG, "  Blink rate: %d", led_settings[led].blink_rate);
            // Handle blink mode
        } else if (strcmp(led_settings[led].display_mode, "fade") == 0) {
            ESP_LOGI(TAG, "  Fade rate: %d", led_settings[led].fade_rate);
            // Handle fade mode
        }

        // Set LED brightness
        // set_led_brightness(led + 1, led_settings[led].brightness);
    }

    // Free the allocated memory
    free(config);
}


