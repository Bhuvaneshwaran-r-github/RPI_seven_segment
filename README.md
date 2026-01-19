# RPI Seven Segment Display Driver

A Linux kernel character device driver for Raspberry Pi 3B+ to control a 4-digit 7-segment display via GPIO pins.

## Features

- Custom Linux kernel module with file operations (open, read, write, release)
- Controls 12 GPIO pins for multiplexed 7-segment display
- User-space applications for interactive number display
- Supports display range 0-9999 with error and off states
- Time-multiplexed digit scanning for smooth display

## Hardware Requirements

- Raspberry Pi 3B+
- 4-digit 7-segment display (common cathode)
- Current limiting resistors
- Connecting wires

## Software Prerequisites

- Raspberry Pi OS (Linux kernel headers required)
- GCC compiler
- Make utility

## Building the Driver

```bash
# Build the kernel module
make

# Load the module
sudo insmod seven_segment_driver_v1.ko

# Create device node (if not auto-created)
sudo mknod /dev/ext_device c <major> 0
```

## Usage

### Running the Application

```bash
# Compile the user application
gcc -o display_number_int display_number_int.c

# Run the application
./display_number_int
```

### Display Commands

- Enter any number (0-9999) to display on the 7-segment
- Type `exit` to turn off display and quit

## File Structure

| File | Description |
|------|-------------|
| `seven_segment_driver_v1.c` | Kernel module driver (int array method) |
| `display_number_int.c` | User-space application for number display |
| `Makefile` | Build configuration |

## How It Works

1. The kernel driver creates a character device at `/dev/ext_device`
2. User-space application opens the device and writes segment patterns
3. Driver configures GPIO pins and outputs the patterns
4. Time-multiplexing scans through all 4 digits rapidly to create a persistent display

## Segment Mapping

The display uses standard 7-segment encoding (a-g segments + decimal point) with 4 digit select lines for multiplexing.
