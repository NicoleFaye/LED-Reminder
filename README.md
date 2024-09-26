# LED Reminder Project

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Hardware Requirements](#hardware-requirements)
4. [Software Dependencies](#software-dependencies)
5. [Installation](#installation)
6. [Configuration](#configuration)
7. [Usage](#usage)
8. [LED Modes](#led-modes)
9. [Troubleshooting](#troubleshooting)
10. [Contributing](#contributing)
11. [License](#license)

## Introduction
The LED Reminder is an ESP32-S3 based device featuring 5 buttons and 5 LEDs. It offers versatile LED control modes and easy configuration through a text file.

## Features
- 5 independently controllable LEDs with corresponding buttons
- Multiple LED operation modes: Manual, Offset, Set Time, Fixed Interval
- Various LED display modes: Solid, Blink, Fade
- Adjustable brightness for each LED
- Easy configuration via text file
- USB Mass Storage support for easy settings file access
- Real-time clock synchronization via NTP

## Hardware Requirements
- LED Reminder device 
or 
- ESP32-S3 development board
- 5 LEDs
- 5 push buttons
- Resistors and wiring as needed

## Software Dependencies
- ESP-IDF v4.4 or later

## Installation
(Provide step-by-step installation instructions)

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
- `ledX_set_time_duration`: Duration in seconds (for set_time mode)
- `ledX_fixed_interval_seconds`: (for fixed_interval mode)

## Usage
1. Power on the device
2. Connect to the device via USB to access the configuration file
3. Edit the `settings.txt` file as needed
4. Safely eject the device and restart it to apply changes

## LED Modes

### Manual Mode
- Short press: Toggle LED on/off
- Long press: Toggle LED on

### Offset Mode
LED turns on automatically after a set time from being turned off

### Set Time Mode
LED turns on at specified times with configurable frequency and duration

### Fixed Interval Mode
LED turns on at fixed intervals from device startup

## Display Modes

### Solid
LED maintains constant brightness when on

### Blink
LED alternates between on and off states

### Fade
LED gradually transitions between on and off states

## Troubleshooting
(List common issues and their solutions)

## Contributing
(Provide guidelines for contributing to the project)

## License
(Specify the project's license)