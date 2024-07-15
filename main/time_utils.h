#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include "esp_log.h"
#include "esp_sntp.h"
#include <time.h>
#include <sys/time.h>
#include "nvs_flash.h"
#include "settings.h"
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

void initialize_sntp(const char*);
void check_daylight_saving(void);
void initialize_nvs(void);
void print_current_time(void);

#ifdef __cplusplus
}
#endif

#endif // TIME_UTILS_H
