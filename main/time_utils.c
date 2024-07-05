#include "time_utils.h"

void initialize_sntp(const char* timezone)
{
    // Set the time zone before initializing SNTP
    // for formats see https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
    //setenv("TZ", "PST8PDT,M3.2.0,M11.1.0", 1); // Set timezone to Eastern Time (US)
    setenv("TZ", timezone, 1); // Set timezone to Eastern Time (US)
    tzset();                                   // Apply the timezone change

    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();
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