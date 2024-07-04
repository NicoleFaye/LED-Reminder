#ifndef SETTINGS_H
#define SETTINGS_H

#define MAX_LINE_LENGTH 256
#define NUM_LEDS 4

typedef struct {
    char key[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];
} KeyValuePair;

extern const char* TAG;
extern const char* settings_txt;
extern const KeyValuePair timezone_dict[];
extern const int timezone_dict_size;
const char* get_timezone(const char* key);

#endif // SETTINGS_H
