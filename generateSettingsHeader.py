import csv

def generate_header_and_source_with_raw_text_and_comments(text_file_path, csv_file_path, header_output_path, source_output_path):
    # Process the text file
    escaped_lines = []
    with open(text_file_path, 'r') as text_file:
        for line in text_file:
            escaped_line = line.replace('\\', '\\\\').replace('"', '\\"').rstrip()
            escaped_lines.append(escaped_line + '\\n')

    # Process the CSV file
    csv_dict = {}
    with open(csv_file_path, 'r') as csv_file:
        csv_reader = csv.reader(csv_file)
        for row in csv_reader:
            key, value = row
            csv_dict[key] = value

    # Convert dictionary to C-style initializer list
    dict_entries = [f'{{"{key}", "{value}"}}' for key, value in csv_dict.items()]
    dict_initializer = '{' + ', '.join(dict_entries) + '}'

    # Write to the output header file
    with open(header_output_path, 'w') as header_file:
        header_file.write('#ifndef SETTINGS_H\n#define SETTINGS_H\n\n')
        header_file.write('#define MAX_LINE_LENGTH 256\n#define NUM_LEDS 4\n\n')
        header_file.write('typedef struct {\n')
        header_file.write('    char key[MAX_LINE_LENGTH];\n')
        header_file.write('    char value[MAX_LINE_LENGTH];\n')
        header_file.write('} KeyValuePair;\n\n')
        header_file.write('extern const char* TAG;\n')
        header_file.write('extern const char* settings_txt;\n')
        header_file.write('extern const KeyValuePair timezone_dict[];\n')
        header_file.write('extern const int timezone_dict_size;\n')
        header_file.write('const char* get_timezone(const char* key);\n\n')
        header_file.write('#endif // SETTINGS_H\n')

    # Write to the output source file
    with open(source_output_path, 'w') as source_file:
        source_file.write('#include "settings.h"\n\n')
        source_file.write('#include <string.h>\n\n')
        source_file.write('const char *TAG = "KD-Indicator";\n')
        source_file.write('const char* settings_txt = "')
        source_file.write(''.join(escaped_lines))
        source_file.write('";\n\n')

        source_file.write('const KeyValuePair timezone_dict[] = ')
        source_file.write(dict_initializer)
        source_file.write(';\n\n')

        source_file.write('const int timezone_dict_size = sizeof(timezone_dict) / sizeof(timezone_dict[0]);\n\n')

        source_file.write('const char* get_timezone(const char* key) {\n')
        source_file.write('    for (int i = 0; i < timezone_dict_size; ++i) {\n')
        source_file.write('        if (strcmp(timezone_dict[i].key, key) == 0) {\n')
        source_file.write('            return timezone_dict[i].value;\n')
        source_file.write('        }\n')
        source_file.write('    }\n')
        source_file.write('    return NULL;\n')
        source_file.write('}\n')

if __name__ == '__main__':
    generate_header_and_source_with_raw_text_and_comments('settings.txt', 'timezones.csv', 'main/settings.h', 'main/settings.c')
