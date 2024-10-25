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

    // Extract the setting name after the LED number
    const char *setting = strchr(kv.key, '_');
    if (setting == NULL)
    {
        return 1; // Skip if there's no underscore in the key
    }
    setting++; // Move past the underscore

    // Use exact matches instead of substring matches
    if (strcmp(setting, "function_mode") == 0)
    {
        if (strcmp(kv.value, "manual") != 0 && strcmp(kv.value, "offset") != 0 &&
            strcmp(kv.value, "set_time") != 0 && strcmp(kv.value, "fixed_interval") != 0 &&
            strcmp(kv.value, "set_days") != 0)
        {
            return 0; // Invalid function mode
        }
    }
    else if (strcmp(setting, "display_mode") == 0)
    {
        if (strcmp(kv.value, "solid") != 0 && strcmp(kv.value, "blink") != 0 &&
            strcmp(kv.value, "fade") != 0)
        {
            return 0; // Invalid display mode
        }
    }
    else if (strcmp(setting, "brightness") == 0)
    {
        int brightness = atoi(kv.value);
        if (brightness < 0 || brightness > 100)
        {
            return 0; // Invalid brightness
        }
    }
    else if (strcmp(setting, "set_time_days") == 0)
    {
        int days = atoi(kv.value);
        if (days < 0 || days > 365 * 10)
        {
            return 0; // Invalid days
        }
    }
    else if (strcmp(setting, "set_time") == 0) // Exact match for set_time
    {
        // Validate the time format (HH:MM)
        int hours, minutes;
        if (sscanf(kv.value, "%d:%d", &hours, &minutes) != 2 ||
            hours < 0 || hours > 23 || minutes < 0 || minutes > 59)
        {
            return 0; // Invalid time format
        }
    }
    else if (strcmp(setting, "set_time_duration") == 0) // Exact match for set_time_duration
    {
        // Validate that the duration is zero or positive
        int duration = atoi(kv.value);
        if (duration < 0)
        {
            return 0; // Invalid duration
        }
    }

    return 1; // Valid setting
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
