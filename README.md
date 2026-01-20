# 🔢 Seven Segment Display Driver for Raspberry Pi

A **Linux kernel character device driver** for Raspberry Pi that controls a 4-digit 7-segment display through GPIO pins.

---

## 📋 Project Description

This project implements a **custom Linux kernel module** that creates a character device for controlling a 7-segment display. It demonstrates deep understanding of Linux kernel programming, device drivers, and hardware interfacing.

### What I Built:
- **Kernel Module**: Loadable kernel module (LKM) with file operations
- **Character Device**: `/dev/ext_device` for user-space communication
- **GPIO Control**: Direct hardware access for 12 GPIO pins
- **User Applications**: Multiple test applications for different display modes

### Real-World Application:
Device drivers are the bridge between hardware and software. This type of driver is used in industrial displays, point-of-sale terminals, embedded HMIs, and IoT devices. Understanding kernel programming is essential for embedded Linux roles.

---

## 🛠️ Technologies Used

| Category | Technology |
|----------|------------|
| **Platform** | Raspberry Pi 3B+ |
| **Kernel** | Linux (Raspbian) |
| **Language** | C (Kernel & User space) |
| **API** | Linux GPIO Subsystem |
| **Driver Type** | Character Device Driver |
| **Build System** | Kbuild / Makefile |
| **Hardware** | 4-digit 7-segment display (Common Cathode) |

---

## 📊 System Architecture

```
    ┌─────────────────────────────────────────────────────┐
    │                   USER SPACE                        │
    │   ┌──────────────────────────────────────────────┐  │
    │   │         User Application (display_*.c)       │  │
    │   │              open() / write() / read()       │  │
    │   └──────────────────┬───────────────────────────┘  │
    ├──────────────────────┼──────────────────────────────┤
    │                      │ /dev/ext_device              │
    │                   KERNEL SPACE                      │
    │   ┌──────────────────┴───────────────────────────┐  │
    │   │         Character Device Driver              │  │
    │   │     (seven_segment_driver_v1.c)              │  │
    │   │                                              │  │
    │   │  ┌─────────┐  ┌─────────┐  ┌─────────┐      │  │
    │   │  │ ext_open│  │ext_write│  │ext_read │      │  │
    │   │  └─────────┘  └─────────┘  └─────────┘      │  │
    │   └──────────────────┬───────────────────────────┘  │
    │                      │                              │
    │   ┌──────────────────┴───────────────────────────┐  │
    │   │            Linux GPIO Subsystem              │  │
    │   │         gpio_set_value() / gpio_get_value()  │  │
    │   └──────────────────┬───────────────────────────┘  │
    └──────────────────────┼──────────────────────────────┘
                           │
    ┌──────────────────────┴───────────────────────────────┐
    │                    HARDWARE                          │
    │     GPIO 16-27  →  7-Segment Display (4 digits)      │
    └──────────────────────────────────────────────────────┘
```

---

## 🔑 Key Skills Demonstrated

### Linux Kernel Programming
- Kernel module initialization (`module_init`, `module_exit`)
- Character device registration (`alloc_chrdev_region`, `cdev_add`)
- Device class and node creation (`class_create`, `device_create`)
- Kernel memory management
- `printk` logging and debugging

### Device Driver Development
- File operations implementation (`open`, `release`, `read`, `write`)
- User-kernel data transfer (`copy_to_user`, `copy_from_user`)
- Error handling with goto-based cleanup
- Proper resource deallocation on exit

### GPIO Subsystem
- GPIO validation and request (`gpio_is_valid`, `gpio_request`)
- Direction configuration (`gpio_direction_output`)
- Value manipulation (`gpio_set_value`, `gpio_get_value`)
- Sysfs export for user-space access (`gpiod_export`)
- GPIO descriptor API usage

### Hardware Interfacing
- 7-segment display multiplexing
- Digit encoding (a-g segments)
- Time-division multiplexing for multi-digit display
- Current limiting and drive considerations

### Build & Deployment
- Makefile for kernel modules
- Module loading/unloading (`insmod`, `rmmod`)
- Kernel message inspection (`dmesg`)
- Device node permissions

---

## 📁 Project Structure

| File | Purpose |
|------|---------|
| `seven_segment_driver_v1.c` | Kernel module with GPIO control |
| `display_number_int.c` | User app for number display |
| `Makefile` | Kernel module build configuration |

---

## 🎯 What I Learned

- Linux kernel internals and module development
- Character device driver architecture
- Safe kernel programming practices
- Hardware abstraction in Linux
- User-kernel space communication mechanisms
