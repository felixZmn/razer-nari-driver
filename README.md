# razer-nari-driver

An attempt to develop an Ubuntu driver for the Razer Nari Ultimate Headset

## Goal

This project has two goals:
1. Gain a better understanding of usb and hardware drivers
2. Develop linux support for my headset (e. g. read battery state, disable rgb light, ...)

An optional goal is the integration of the Headset into the Ubuntu power settings section, like shown in the screenshot: 

![Image of the Ubuntu power settings section](https://raw.githubusercontent.com/felixZmn/razer-nari-driver/main/images/power%20settings.png)

The audio-part of the headset works flawless, thanks to [this](https://github.com/imustafin/razer-nari-pulseaudio-profile) Project.

## Problems
### Solved
- [x] Activate/deactivate mic by writing 0 or 1 from userspace to the created sysfs-File
- [x] Save the mic active state when the mute button on the headset is pressed

### Open
- [ ] sysfs files permissions
- [ ] Control mic sensitivity
- [ ] Control haptic feedback
- [ ] Control rgb light
- [ ] Report battery/charing state (maybe `linux/power_supply.h` could be useful?)

## Unresolved problems
1. I was not able to reverse engineer the reports send from the driver to the headset. The data sent between driver and Headset is seemingly random. Because of that, I couldn't get any actual information about the state of the headset (Is it charging? What's the current color and color effect? etc.).
2. The data send from the driver to the headset is seemingly decoded. For example, the microphone sensitivity is controlled by an 4 char hex value. But I couldn't figure out, how the encoding algorithm is working.

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
