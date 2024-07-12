#ifndef MSC_UTILS_H
#define MSC_UTILS_H

#include "esp_log.h"
#include "esp_err.h"
#include "dirent.h"
#include "errno.h"
#include "tinyusb.h"
#include "esp_vfs_fat.h"
#include "esp_partition.h"
#include "tusb_msc_storage.h"
#include "settings.h"

#include <stdio.h>
#include <stdlib.h> // For malloc and free

#ifdef __cplusplus
extern "C" {
#endif

// TinyUSB descriptors
#define EPNUM_MSC 1
#define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_MSC_DESC_LEN)

enum {
    ITF_NUM_MSC = 0,
    ITF_NUM_TOTAL
};

enum {
    EDPT_CTRL_OUT = 0x00,
    EDPT_CTRL_IN = 0x80,
    EDPT_MSC_OUT = 0x01,
    EDPT_MSC_IN = 0x81,
};


#define BASE_PATH "/data" // base path to mount the partition

// Function prototypes
void _mount(void);
void expose_msc(void);
void initialize_msc(void);
void storage_mount_changed_cb(tinyusb_msc_event_t *event);
esp_err_t storage_init_spiflash(wl_handle_t *wl_handle);
int create_settings_file(void);

void read_settings_file(void);

extern tusb_desc_device_t descriptor_config;
extern char const *string_desc_arr[];
extern const uint8_t desc_configuration[];

#ifdef __cplusplus
}
#endif

#endif // MSC_UTILS_H