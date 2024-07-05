#ifndef REPL_H
#define REPL_H

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_check.h"
#include "settings.h"
#include "msc_utils.h"


#define PROMPT_STR CONFIG_IDF_TARGET

int console_read(int argc, char **argv);
int console_unmount(int argc, char **argv);
int console_write(int argc, char **argv);
int console_size(int argc, char **argv);
int console_status(int argc, char **argv);
int console_exit(int argc, char **argv);

void initialize_repl(void);

#endif 