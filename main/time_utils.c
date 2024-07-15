#include "time_utils.h"

void initialize_sntp(const char* timezone)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    
    // Set the time zone
    setenv("TZ", timezone, 1);
    tzset();

    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();

    // Wait for time to be set
    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    const int retry_count = 15;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    time(&now);
    localtime_r(&now, &timeinfo);

    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGE(TAG, "System time NOT set.");
    } else {
        ESP_LOGI(TAG, "System time is set.");
        print_current_time();
    }
}

void print_current_time(void)
{
    time_t now;
    struct tm timeinfo;
    char strftime_buf[64];

    time(&now);
    localtime_r(&now, &timeinfo);

    // Format the time string
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

    // Print the formatted time
    ESP_LOGI(TAG, "Current time: %s", strftime_buf);
}

void check_daylight_saving(void)
{

    // Wait for time to be set
    time_t now = 0;
    struct tm timeinfo = {0};
    while (timeinfo.tm_year < (2016 - 1900))
    { // Use a recent past year as a sanity check
        time(&now);
        localtime_r(&now, &timeinfo);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    ESP_LOGI("TIME", "Current time: %s", asctime(&timeinfo));
    if (timeinfo.tm_isdst > 0)
    {
        ESP_LOGI("TIME", "Daylight Saving Time is currently in effect.");
    }
    else
    {
        ESP_LOGI("TIME", "Standard Time is currently in effect.");
    }
}

void initialize_nvs(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
}