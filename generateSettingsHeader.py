def generate_header_with_raw_text_and_comments(file_path, output_path):
    escaped_lines = []
    with open(file_path, 'r') as file:
        for line in file:
            # Escape double quotes and backslashes
            escaped_line = line.replace('\\', '\\\\').replace('"', '\\"').rstrip()
            # Add newline character to preserve formatting
            escaped_lines.append(escaped_line + '\\n')

    with open(output_path, 'w') as file:
        file.write('#ifndef SETTINGS_H\n#define SETTINGS_H\n\n')
        file.write('const char* settings_txt = "')
        # Join all lines into one long string
        file.write(''.join(escaped_lines))
        file.write('";\n\n#endif // SETTINGS_H\n')

if __name__ == '__main__':
    generate_header_with_raw_text_and_comments('settings.txt', 'main/settings.h')