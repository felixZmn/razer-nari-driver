//
// Created by felix on 23.12.22.
//

#include "nari.h"
#include <linux/hid.h>
#include <linux/usb.h>

/**
 * internal method to set mute state of the headset
 * @param dev pointer to the device
 * @param state mute: 0x0001; unmute: 0x0000;
 */
void set_mute(struct device *dev, int state) {
    struct usb_interface *intf = to_usb_interface(dev->parent);
    struct usb_device *usbdev = interface_to_usbdev(intf);
    usb_control_msg(usbdev, usb_sndctrlpipe(usbdev, 0), 1, 0x21, 0x0100, 0x0300, &state, 1, USB_CTRL_SET_TIMEOUT);
}

void mute_mic(struct device *dev) {
    int foo = 0x0001;
    set_mute(dev, foo);
}

void unmute_mic(struct device *dev) {
    int foo = 0x0000;
    set_mute(dev, foo);
}
