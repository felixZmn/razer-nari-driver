# razer-nari-driver

An attempt to develop an Ubuntu driver for the Razer Nari Ultimate Headset

## Goal

The main goal of this Project was to learn how to develop a linux hardware driver. For this, I wanted to integrate my Razer headset, just like some of my other devices are, into the ubuntu power settings section.

![Image of the Ubuntu power settings section](https://raw.githubusercontent.com/felixZmn/razer-nari-driver/main/images/power%20settings.png)

The audio-part of the headset works flawless, thanks to [this](https://github.com/imustafin/razer-nari-pulseaudio-profile) Project.

## Solved Problems

It is possible to set the mute state of the headset by writing 0 or 1 to the created sysfs-File.

All other possible settings (for example color effects, haptic feedback, ...) could be implemented the same way: Capture the original driver traffic and rebuild the feature in this driver.

## Unresolved problems

There are two large unresolved problems.

1. If one is setting the mute state of the mic by pressing the mute Button on the headset, an interrupt is sent to the driver. I was not able to react to this interrupt, because I couldn't find sufficient documentation on how to achieve that.
2. I was not able to reverse engineer the reports send from the driver to the headset. The data sent between driver and Headset is seemingly random. Because of that, I couldn't get any actual information about the state of the headset (Is it charging? What's the current color and color effect? etc.).

## Summary

To wrap it up, the attempt to create a fully working driver and integrate it into the ubuntu power section was not successful. Nevertheless, the main goal, to learn about the Linux Kernel Module development, was achieved.

## Further Reading

[USB in a NutShell](https://www.beyondlogic.org/usbnutshell/usb1.shtml#Introduction)\
[HOWTO do Linux kernel development](https://www.kernel.org/doc/html/v4.16/process/howto.html)\
[Linux Device Drivers, Third Edition](https://lwn.net/Kernel/LDD3/)

## Installing (WIP!!)

To build and install the driver, following steps are necessary (Ubuntu):

1. Install required dependencies: \
   `sudo apt install linux-headers-$(uname -r) libusb-dev`
2. Install driver\
   `sudo make all`

To uninstall the driver module, the following command can be used:
`sudo make uninstall`
