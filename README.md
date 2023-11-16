# Razer Nari Driver

This is a prototypical (buggy! incomplete!) implementation of a driver for the Razer Nari Ultimate headset. It is based on the [Openrazer-Project](https://github.com/openrazer/openrazer).

## Status Quo

The Razer Nari Headset lacks Linux support. Sound and microphone work (nearly, you need [this](https://github.com/imustafin/razer-nari-pulseaudio-profile) audio profile) out of the box, the other features (rgb light, haptic feedback, ...) are not controllable. The settings are saved in the headset itself, so you can configure it on a Windows machine and use it on Linux. It would be nice to have a Linux driver, so you can configure the device on such systems too.

## Goals

This project has two main goals:

1. Develop an usable driver for the Razer Nari Headset.\
   My personal main goal is to read the battery state of the headset, so that I can integrate it into the Ubuntu power settings section like shown in the screenshot.
   ![Image of the Ubuntu power settings section](https://raw.githubusercontent.com/felixZmn/razer-nari-driver/main/images/power%20settings.png)
2. Learn more about usb and hardware drivers in general.\
   I'm generally interested in hardware and low-level programming, so this project is a great opportunity to learn more about it.

## Problems

### Solved

-   [x] Activate/deactivate mic by writing 0 or 1 from userspace to the created sysfs-File
-   [x] Save the mic active state when the mute button on the headset is pressed

### Open

-   [ ] sysfs files permissions
-   [ ] Control mic sensitivity
-   [ ] Control haptic feedback
-   [ ] Control rgb light
-   [ ] Report battery/charing state (maybe `linux/power_supply.h` could be useful?)

## Unresolved problems

1. I was unable to reverse engineer the reports sent from the headset to the driver. The data sent between them seems to be random. I could not find any pattern or structure in the data. That's not to say there isn't a pattern, but I just couldn't find one. The consequence of this is, that I can't read status information (battery state, current color and effect, ...) from the headset.
2. The data send from the driver to the headset is seemingly encoded. For example, the microphone sensitivity is controlled by an seemingly random 4 char hex value, which represents a sensitivity value from 0 to 100. Because I could not find an efficient mapping algorithm, I can only use a lookup table to map the values. This is not a big problem, but it's not very elegant, and therefore not fully implemented.

## Further Reading

[USB in a NutShell](https://www.beyondlogic.org/usbnutshell/usb1.shtml#Introduction)\
[HOWTO do Linux kernel development](https://www.kernel.org/doc/html/v4.16/process/howto.html)\
[Writing USB Device Drivers](https://www.kernel.org/doc/html/v4.16/driver-api/usb/writing_usb_driver.html)\
[Linux Device Drivers, Third Edition](https://lwn.net/Kernel/LDD3/)

## Installing (WIP!!)

To build and install the driver, following steps are necessary (Ubuntu):

1. Install required dependencies: \
   `sudo apt install linux-headers-$(uname -r) libusb-dev`
2. Install driver\
   `sudo make all`

To uninstall the driver module, the following command can be used:
`sudo make uninstall`
