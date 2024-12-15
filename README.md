# LED Reminder

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Hardware Requirements](#hardware-requirements)
4. [Software Dependencies](#software-dependencies)
5. [Configuration](#configuration)
6. [Usage](#usage)
7. [LED Modes](#led-modes)
8. [Troubleshooting](#troubleshooting)
9. [Contributing](#contributing)
10. [License](#license)

## Introduction
The LED Reminder is a, ESP32-S3-based device designed to serve as a versatile and user-friendly reminder system. It features five buttons and five corresponding LEDs, each of which can be independently configured and controlled. The device is highly customizable, allowing users to set up reminders, timers, and visual cues for various purposes.

## Features
- 5 independently controllable LEDs with corresponding buttons
- Multiple LED operation modes: Manual, Offset, Set Time, Fixed Interval
- Various LED display modes: Solid, Blink, Fade
- Adjustable brightness for each LED
- Easy configuration via text file
- Real-time clock synchronization via NTP over WiFi

## Hardware Requirements
- LED Reminder device 
or 
- ESP32-S3 development board
- 5 LEDs
- 5 push buttons
- Resistors and wiring as needed

## Software Dependencies
- ESP-IDF v5.2.3

## Configuration
The device is configured using a `settings.txt` file. Key settings include:

- WiFi credentials
- Timezone
- LED operation modes
- LED display modes
- Brightness levels
- Timing settings

### LED-specific Settings
For each LED (1-5), the following can be configured:
- `ledX_function_mode`: manual, offset, set_time, fixed_interval
- `ledX_display_mode`: solid, blink, fade
- `ledX_brightness`: 0-100
- `ledX_offset_seconds`: (for offset mode)
- `ledX_set_time`: HH:MM (for set_time mode)
- `ledX_set_time_days`: Interval in days (for set_time mode)
- `led4_set_days`: Days of the week (for set_days mode)
- `ledX_set_time_duration`: Duration in seconds (for set_time mode)
- `ledX_fixed_interval_seconds`: (for fixed_interval mode)

## Usage
1. Connect to the device via USB to a computer to access the configuration file
2. Edit the `settings.txt` file as needed
3. Safely eject the device and restart it to apply changes

- You can delete the `settings.txt` file to reset to defaults on next boot
- Upon connecting to wifi, all 5 LEDs will blink 3 times to indicate successful connection

## LED Modes

### Manual Mode
- Short press: Toggle LED on/off
- Long press: Toggle LED on

### Set Time Mode
LED turns on at specified times with configurable frequency and duration

### Set Days Mode
LED turns on at specified times on specific days of the week

### Offset Mode
LED turns on automatically after a set time from being turned off by pressing the button

### Fixed Interval Mode
LED turns on at fixed intervals from device startup, independent of when the button was pressed

## Display Modes

### Solid
LED maintains constant brightness when on

### Blink
LED alternates between on and off states

### Fade
LED gradually transitions between on and off states

## Troubleshooting
- The device may not boot if the `settings.txt` file is invalid or corrupted
- If the device fails to connect to WiFi, ensure the SSID matches exactly and the password is correct
- If you come across any other issues, please create and issue on [Github](https://github.com/NicoleFaye/LED-Reminder/issues)
- If you are trying to create your own device, create an issue with any problems you encounter and I will try to help

## Contributing
If you have any suggestions or improvements, please feel free to create a pull request or an issue with your ideas.

## License
This project is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License (CC BY-NC-SA 4.0).

You are free to:
- Share — copy and redistribute the material in any medium or format
- Adapt — remix, transform, and build upon the material

Under the following terms:
- Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made.
- NonCommercial — You may not use the material for commercial purposes.
- ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

For more details, see the [full license text](https://github.com/NicoleFaye/LED-Reminder/blob/main/LICENSE).