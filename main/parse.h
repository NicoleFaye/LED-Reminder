#ifndef PARSE_H
#define PARSE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "settings.h"
#include "esp_log.h"

void trim_whitespace(char* str);
int parse_line(char* line, KeyValuePair* kv);
int validate_led_setting(KeyValuePair kv, int led_num);
int check_configuration(const char* filename);

#endif // PARSE_H
