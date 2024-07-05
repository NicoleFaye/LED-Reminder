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
#include "parse.h"
#include "wifi_utils.h"
#include "time_utils.h"
#include "msc_utils.h"
#include "repl.h"
#include "blink.h"

static bool msc_exposed = true;
static bool repl_enabled = false;



void app_main(void)
{
    //initialize LED on pin 6
    gpio_reset_pin(6);
    gpio_set_direction(6, GPIO_MODE_OUTPUT);

    //ESP_LOGI(TAG, "%s", get_timezone("Africa/Djibouti"));


    // Initialize NVS
    ESP_LOGI(TAG, "Initializing NVS...");
    initialize_nvs();

    // Initialize SNTP
    ESP_LOGI(TAG, "Initializing SNTP...");
    initialize_sntp("PST8PDT,M3.2.0,M11.1.0");


    ESP_LOGI(TAG, "Initializing storage...");
    initialize_msc();

    ESP_LOGI(TAG,"Config check return value: %i",check_configuration(BASE_PATH "/settings.txt"));

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
}
