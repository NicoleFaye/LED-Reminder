#include "repl.h"

const esp_console_cmd_t cmds[] = {
    {
        .command = "read",
        .help = "read BASE_PATH/settings.txt and print its contents",
        .hint = NULL,
        .func = &console_read,
    },
    {
        .command = "write",
        .help = "create file BASE_PATH/settings.txt if it does not exist",
        .hint = NULL,
        .func = &console_write,
    },
    {
        .command = "size",
        .help = "show storage size and sector size",
        .hint = NULL,
        .func = &console_size,
    },
    {
        .command = "expose",
        .help = "Expose Storage to Host",
        .hint = NULL,
        .func = &console_unmount,
    },
    {
        .command = "status",
        .help = "Status of storage exposure over USB",
        .hint = NULL,
        .func = &console_status,
    },
    {
        .command = "exit",
        .help = "exit from application",
        .hint = NULL,
        .func = &console_exit,
    }};

// unmount storage
int console_unmount(int argc, char **argv)
{
    if (tinyusb_msc_storage_in_use_by_usb_host())
    {
        ESP_LOGE(TAG, "storage is already exposed");
        return -1;
    }
    ESP_LOGI(TAG, "Unmount storage...");
    ESP_ERROR_CHECK(tinyusb_msc_storage_unmount());
    return 0;
}

// read BASE_PATH/settings.txt and print its contents
int console_read(int argc, char **argv)
{
    if (tinyusb_msc_storage_in_use_by_usb_host())
    {
        ESP_LOGE(TAG, "storage exposed over USB. Application can't read from storage.");
        return -1;
    }
    ESP_LOGD(TAG, "read from storage:");
    const char *filename = BASE_PATH "/settings.txt";
    FILE *ptr = fopen(filename, "r");
    if (ptr == NULL)
    {
        ESP_LOGE(TAG, "Filename not present - %s", filename);
        return -1;
    }
    char buf[1024];
    while (fgets(buf, 1000, ptr) != NULL)
    {
        printf("%s", buf);
    }
    fclose(ptr);
    return 0;
}

// create file BASE_PATH/settings.txt if it does not exist
int console_write(int argc, char **argv)
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

// Show storage size and sector size
int console_size(int argc, char **argv)
{
    if (tinyusb_msc_storage_in_use_by_usb_host())
    {
        ESP_LOGE(TAG, "storage exposed over USB. Application can't access storage");
        return -1;
    }
    uint32_t sec_count = tinyusb_msc_storage_get_sector_count();
    uint32_t sec_size = tinyusb_msc_storage_get_sector_size();
    printf("Storage Capacity %lluMB\n", ((uint64_t)sec_count) * sec_size / (1024 * 1024));
    return 0;
}

// exit from application
int console_status(int argc, char **argv)
{
    printf("storage exposed over USB: %s\n", tinyusb_msc_storage_in_use_by_usb_host() ? "Yes" : "No");
    return 0;
}

// exit from application
int console_exit(int argc, char **argv)
{
    tinyusb_msc_unregister_callback(TINYUSB_MSC_EVENT_MOUNT_CHANGED);
    tinyusb_msc_storage_deinit();
    printf("Application Exiting\n");
    exit(0);
    return 0;
}

void initialize_repl(void)
{
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    // Prompt to be printed before each line.
    // This can be customized, made dynamic, etc.
    repl_config.prompt = PROMPT_STR ">";
    repl_config.max_cmdline_length = 64;
    esp_console_register_help_command();
    esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));
    for (int count = 0; count < sizeof(cmds) / sizeof(esp_console_cmd_t); count++)
    {
        ESP_ERROR_CHECK(esp_console_cmd_register(&cmds[count]));
    }
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
}