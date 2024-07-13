#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include "settings.h"
#include "parse.h"
#include "wifi_utils.h"
#include "time_utils.h"
#include "led_utils.h"
#include "esp_log.h"

/**
 * @brief Apply the parsed configuration to the system
 * 
 * This function reads the configuration file, parses it, and applies
 * the settings to various subsystems (WiFi, LED, Time, etc.).
 */
void apply_configuration(void);

#endif // CONFIG_HANDLER_H
