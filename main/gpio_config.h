#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include "driver/gpio.h"
#include "esp_log.h"
#include "led_utils.h"
#include "buttons.h"

// Strapping pins (must not be pulled low during boot)
#define STRAPPING_PIN_0     0   // Used for Boot selection
#define STRAPPING_PIN_3     3   // Used for UART Download selection
#define STRAPPING_PIN_45    45  // Used for Boot selection
#define STRAPPING_PIN_46    46  // Used for Boot selection

// SPI Flash and PSRAM (these pins are not available for general use)
#define FLASH_START_PIN     26  // Flash starts at GPIO26
#define FLASH_END_PIN      32   // Flash ends at GPIO32
#define PSRAM_START_PIN    33   // PSRAM starts at GPIO33
#define PSRAM_END_PIN      37   // PSRAM ends at GPIO37

// JTAG pins (used for debugging)
#define JTAG_START_PIN     19   // JTAG starts at GPIO19
#define JTAG_END_PIN       22   // JTAG ends at GPIO22

// USB pins (not available for general use)
#define USB_DM_PIN         19   // USB D-
#define USB_DP_PIN         20   // USB D+

// Other unavailable pins on ESP32-S3-WROOM-1-N4:
// GPIO22-25: not exposed
// GPIO26-32: connected to SPI flash
// GPIO33-37: connected to PSRAM
// GPIO19-20: USB
// GPIO0, 3, 45, 46: strapping pins

void configure_unused_pins(void);
bool is_special_pin(int pin);

#endif