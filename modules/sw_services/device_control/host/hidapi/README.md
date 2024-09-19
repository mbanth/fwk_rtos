# File structure

This directory contains the essential files required to build HID drivers using HIDAPI for Win32 host applications. The original source files are available in the [HIDAPI library version 0.14.0](https://github.com/libusb/hidapi/releases/tag/hidapi-0.14.0).

# Local changes

The files from the HIDAPI library can be used without modifications for Linux and macOS. However, for Windows, it is necessary to select the correct Usage Page when opening the device. The list of changes made for Windows compatibility can be found in the `windows_patch_v0.14.0.patch`.
