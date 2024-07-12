#include "msc_utils.h"

/* TinyUSB descriptors
 ********************************************************************* */
#define EPNUM_MSC 1
#define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_MSC_DESC_LEN)

uint8_t const desc_configuration[] = {
    // Config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, EP Out & EP In address, EP size
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 0, EDPT_MSC_OUT, EDPT_MSC_IN, TUD_OPT_HIGH_SPEED ? 512 : 64),
};

tusb_desc_device_t descriptor_config = {
    .bLength = sizeof(descriptor_config),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x303A, // This is Espressif VID. This needs to be changed according to Users / Customers
    .idProduct = 0x4002,
    .bcdDevice = 0x100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01};

char const *string_desc_arr[] = {
    (const char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
    "TinyUSB",                  // 1: Manufacturer
    "TinyUSB Device",           // 2: Product
    "123456",                   // 3: Serials
    "Example MSC",              // 4. MSC
};

/*********************************************************************** TinyUSB descriptors*/

// mount the partition and show all the files in BASE_PATH
void _mount(void)
{
    ESP_LOGI(TAG, "Mount storage...");
    ESP_ERROR_CHECK(tinyusb_msc_storage_mount(BASE_PATH));

    // List all the files in this directory
    ESP_LOGI(TAG, "ls command output:");
    struct dirent *d;
    DIR *dh = opendir(BASE_PATH);
    if (!dh)
    {
        if (errno == ENOENT)
        {
            // If the directory is not found
            ESP_LOGE(TAG, "Directory doesn't exist %s", BASE_PATH);
        }
        else
        {
            // If the directory is not readable then throw error and exit
            ESP_LOGE(TAG, "Unable to read directory %s", BASE_PATH);
        }
        return;
    }
    // While the next entry is not readable we will print directory files
    while ((d = readdir(dh)) != NULL)
    {
        ESP_LOGI(TAG, "%s", d->d_name);
    }
    ESP_LOGI(TAG, "------------");
    return;
}

// callback that is delivered when storage is mounted/unmounted by application.
void storage_mount_changed_cb(tinyusb_msc_event_t *event)
{
    ESP_LOGI(TAG, "Storage mounted to application: %s", event->mount_changed_data.is_mounted ? "Yes" : "No");
}

esp_err_t storage_init_spiflash(wl_handle_t *wl_handle)
{
    ESP_LOGI(TAG, "Initializing wear levelling");

    const esp_partition_t *data_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, NULL);
    if (data_partition == NULL)
    {
        ESP_LOGE(TAG, "Failed to find FATFS partition. Check the partition table.");
        return ESP_ERR_NOT_FOUND;
    }

    return wl_mount(data_partition, wl_handle);
}

void expose_msc(void)
{
    ESP_LOGI(TAG, "USB MSC initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = &descriptor_config,
        .string_descriptor = string_desc_arr,
        .string_descriptor_count = sizeof(string_desc_arr) / sizeof(string_desc_arr[0]),
        .external_phy = false,
        .configuration_descriptor = desc_configuration,
    };
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB MSC initialization DONE");
}

void initialize_msc(void){
    // Initialize wear levelling
    static wl_handle_t wl_handle = WL_INVALID_HANDLE;
    ESP_ERROR_CHECK(storage_init_spiflash(&wl_handle));

    // Initialize MSC storage
    const tinyusb_msc_spiflash_config_t config_spi = {
        .wl_handle = wl_handle,
        .callback_mount_changed = storage_mount_changed_cb, /* First way to register the callback. This is while initializing the storage. */
        .mount_config.max_files = 5,
    };
    ESP_ERROR_CHECK(tinyusb_msc_storage_init_spiflash(&config_spi));
    ESP_ERROR_CHECK(tinyusb_msc_register_callback(TINYUSB_MSC_EVENT_MOUNT_CHANGED, storage_mount_changed_cb)); /* Other way to register the callback i.e. registering using separate API. If the callback had been already registered, it will be overwritten. */

    // Mounted in the app by default
    _mount();

    // create the settings.txt file if it does not exist

}


// create file BASE_PATH/settings.txt if it does not exist
int create_settings_file(void)
{
    if (tinyusb_msc_storage_in_use_by_usb_host())
    {
        ESP_LOGE(TAG, "storage exposed over USB. Application can't write to storage.");
        return -1;
    }
    ESP_LOGD(TAG, "write to storage:");
    const char *filename = BASE_PATH "/settings.txt";
    FILE *fd = fopen(filename, "r");
    if (!fd)
    {
        ESP_LOGW(TAG, "settings.txt doesn't exist yet, creating");
        fd = fopen(filename, "w");
        fprintf(fd, settings_txt);
        fclose(fd);
    }
    return 0;
}


void read_settings_file(void) {
    const char *filename = BASE_PATH "/settings.txt";
    FILE *file = fopen(filename, "r"); // Open the file for reading
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open %s for reading", filename);
        return;
    }

    // Assuming you want to read the whole file into a single string
    fseek(file, 0, SEEK_END); // Go to the end of the file
    long fsize = ftell(file); // Get the size of the file
    fseek(file, 0, SEEK_SET); // Go back to the beginning of the file

    char *string = malloc(fsize + 1); // Allocate memory for the file content plus null terminator
    fread(string, 1, fsize, file); // Read the entire file into memory
    string[fsize] = '\0'; // Null-terminate the string

    // Now you can use `string` as needed
    ESP_LOGI(TAG, "File content:\n%s", string);

    free(string); // Don't forget to free the allocated memory
    fclose(file); // Close the file
}