#include "parse.h"

void trim_whitespace(char* str) {
    char* end;

    while(*str == ' ' || *str == '\t' || *str == '\n') str++;

    if(*str == 0) return;

    end = str + strlen(str) - 1;
    while(end > str && (*end == ' ' || *end == '\t' || *end == '\n')) end--;

    end[1] = '\0';
}

int parse_line(char* line, KeyValuePair* kv) {
    char* delimiter = strchr(line, '=');
    if (delimiter == NULL) {
        return 0; // Invalid line format
    }

    *delimiter = '\0';
    strncpy(kv->key, line, MAX_LINE_LENGTH);
    strncpy(kv->value, delimiter + 1, MAX_LINE_LENGTH);

    trim_whitespace(kv->key);
    trim_whitespace(kv->value);

    return 1;
}

int validate_led_setting(KeyValuePair kv, int led_num) {
    if (strncmp(kv.key, "led", 3) != 0) {
        return 1; // Not an LED setting
    }

    char led_id[3];
    snprintf(led_id, 3, "%d", led_num);

    if (strstr(kv.key, led_id) == NULL) {
        return 1; // Not for this LED
    }

    if (strstr(kv.key, "function_mode") != NULL) {
        if (strcmp(kv.value, "manual") != 0 && strcmp(kv.value, "offset") != 0 &&
            strcmp(kv.value, "set_time") != 0 && strcmp(kv.value, "fixed_interval") != 0) {
            return 0; // Invalid function mode
        }
    } else if (strstr(kv.key, "display_mode") != NULL) {
        if (strcmp(kv.value, "solid") != 0 && strcmp(kv.value, "blink") != 0 &&
            strcmp(kv.value, "fade") != 0) {
            return 0; // Invalid display mode
        }
    } else if (strstr(kv.key, "brightness") != NULL) {
        int brightness = atoi(kv.value);
        if (brightness < 0 || brightness > 100) {
            return 0; // Invalid brightness
        }
    } else {
        // Other LED settings can be validated as needed
    }

    return 1;
}

int check_configuration(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        ESP_LOGE(TAG, "Failed to open file: %s", filename);
        return 69;
    }

    char line[MAX_LINE_LENGTH];
    KeyValuePair kv;

    int valid_timezone = 0;
    int valid_ssid = 0;
    int valid_password = 0;
    int valid_leds[NUM_LEDS] = {0};

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        if (!parse_line(line, &kv)) {
            continue;
        }

        if (strcmp(kv.key, "timezone") == 0) {
            if (get_timezone(kv.value) != NULL) {
                valid_timezone = 1;
            } else {
                ESP_LOGE(TAG, "Invalid timezone: %s", kv.value);
            }
        } else if (strcmp(kv.key, "ssid") == 0) {
            valid_ssid = 1;
        } else if (strcmp(kv.key, "password") == 0) {
            valid_password = 1;
        } else {
            for (int i = 1; i <= NUM_LEDS; ++i) {
                if (!validate_led_setting(kv, i)) {
                    ESP_LOGE(TAG, "Invalid setting for LED %d: %s = %s", i, kv.key, kv.value);
                    valid_leds[i-1] = 0;
                } else {
                    valid_leds[i-1] = 1;
                }
            }
        }
    }

    fclose(file);

    if (!valid_timezone) {
        ESP_LOGE(TAG, "Invalid or missing timezone setting.");
        return 1;
    }

    if (!valid_ssid) {
        ESP_LOGE(TAG, "Invalid or missing Wi-Fi SSID setting.");
        return 2;
    }

    if (!valid_password) {
        ESP_LOGE(TAG, "Invalid or missing Wi-Fi password setting.");
        return 3;
    }

    for (int i = 0; i < NUM_LEDS; ++i) {
        if (!valid_leds[i]) {
            ESP_LOGE(TAG, "Invalid settings for LED %d.", i + 1);
            return 4+i;
        }
    }

    ESP_LOGI(TAG, "Configuration file is valid.");
    return 0;
}
