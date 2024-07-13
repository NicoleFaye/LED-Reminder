#include "driver/gpio.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include <time.h>
#include <sys/time.h>

#include "settings.h"
#include "wifi_utils.h"
#include "time_utils.h"
#include "msc_utils.h"
#include "repl.h"
#include "led_utils.h"
#include "buttons.h"
#include "config_handler.h"

static bool msc_exposed = true;
static bool repl_enabled = false;

void app_main(void)
{

    // Initialize buttons
    ESP_LOGI(TAG, "Initializing buttons...");
    initialize_buttons();

    // ESP_LOGI(TAG, "%s", get_timezone("Africa/Djibouti"));

    // Initialize NVS
    ESP_LOGI(TAG, "Initializing NVS...");
    initialize_nvs();


    // Initialize storage
    ESP_LOGI(TAG, "Initializing storage...");
    initialize_msc();

    // Create settings file if it does not exist
    ESP_LOGI(TAG, "Creating settings file...");
    create_settings_file();

    ESP_LOGI(TAG, "Config check return value: %i", check_configuration(BASE_PATH "/settings.txt"));

    // Initialize USB MSC
    if (msc_exposed)
    {
        ESP_LOGI(TAG, "Exposing MSC...");
        expose_msc();
    }

    // Initialize REPL
    if (repl_enabled)
    {
        ESP_LOGI(TAG, "Initializing REPL...");
        initialize_repl();
    }

    // Apply configuration
    ESP_LOGI(TAG, "Applying configuration...");
    apply_configuration();

    // Initialize LED
    ESP_LOGI(TAG, "Initializing LED...");
    initialize_led();

    // Initialize WiFi
    ESP_LOGI(TAG, "Initializing WiFi...");
    wifi_init();

    // Initialize SNTP
    ESP_LOGI(TAG, "Initializing SNTP...");
    initialize_sntp("PST8PDT,M3.2.0,M11.1.0");
}
