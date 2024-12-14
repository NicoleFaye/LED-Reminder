set(priv_requires fatfs console)

if(CONFIG_EXAMPLE_STORAGE_MEDIA_SPIFLASH)
    list(APPEND priv_requires wear_levelling esp_partition)
endif()

idf_component_register(
    SRCS "gpio_config.c" "led_utils.c" "config_handler.c" "tusb_msc_main.c" "parse.c" "settings.c" "wifi_utils.c" "time_utils.c" "msc_utils.c" "repl.c" "buttons.c" "config_handler.c"
    INCLUDE_DIRS .
    PRIV_REQUIRES "${priv_requires}"
    REQUIRES esp_wifi nvs_flash esp_driver_ledc esp_driver_gpio
)
