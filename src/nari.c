//
// Created by felix on 23.12.22.
//

#include "nari.h"
#include <linux/hid.h>
#include <linux/usb.h>

static const int values[] = {0xc000, 0xc9a0, 0xcf79, 0xd3b0, 0xd6fb, 0xd9b0, 0xdbfc, 0xddfa, 0xdfbe, 0xe152, 0xe2c0,
                             0xe40f, 0xe543, 0xe661, 0xe76b, 0xe864, 0xe94e, 0xea2b, 0xeafc, 0xebc2, 0xec7f, 0xed32,
                             0xedde, 0xee83, 0xef21, 0xefb9, 0xf04b, 0xf0d8, 0xf160, 0xf1e3, 0xf262, 0xf2dd, 0xf354,
                             0xf3c8, 0xf439, 0xf4a6, 0xf510, 0xf577, 0xf5dc, 0xf63e, 0xf69e, 0xf6fc, 0xf757, 0xf7b0,
                             0xf807, 0xf85d, 0xf8b0, 0xf902, 0xf952, 0xf9a0, 0xf9ed, 0xfa38, 0xfa82, 0xfacb, 0xfb12,
                             0xfb58, 0xfb9d, 0xfbe0, 0xfc23, 0xfc64, 0xfca4, 0xfce3, 0xfd21, 0xfd5e, 0xfd9b, 0xfdd6,
                             0xfe10, 0xfe4a, 0xfe83, 0xfeba, 0xfef2, 0xff28, 0xff5d, 0xff92, 0xffc6, 0xfffa, 0x002c,
                             0x0060, 0x0095, 0x00ca, 0x0100, 0x0137, 0x016f, 0x01a7, 0x01e1, 0x021b, 0x0256, 0x0292,
                             0x02cf, 0x030d, 0x034c, 0x038c, 0x03cc, 0x040f, 0x0452, 0x0496, 0x04dc, 0x0523, 0x056b,
                             0x05b5, 0x0600};

/**
 * set active state of the headset microphone
 * @param dev pointer to the device
 * @param state off/muted: 0x0001; on/unmuted: 0x0000;
 */
void set_mute(struct device *dev, int state) {
    if (state != 0x00 && state != 0x01) {
        // wrong values provided, do nothing
        return;
    }
    struct usb_interface *intf = to_usb_interface(dev->parent);
    struct usb_device *usbdev = interface_to_usbdev(intf);
    usb_control_msg(usbdev, usb_sndctrlpipe(usbdev, 0), 1, 0x21, 0x0100, 0x0300, &state, 1, USB_CTRL_SET_TIMEOUT);
}

void set_mic_volume(struct device *dev, int percentage) {
    if (percentage < 0 || percentage > 100) {
        // wrong values provided, do nothing
        return;
    }
    struct usb_interface *intf = to_usb_interface(dev->parent);
    struct usb_device *usbdev = interface_to_usbdev(intf);
    int loudness = percentage_to_loudness(percentage);
    printk(KERN_WARNING "loudness: %X\n", loudness);
    usb_control_msg(usbdev, usb_sndctrlpipe(usbdev, 0), 1, 0x21, 0x0200, 0x0300, &loudness, 2, USB_CTRL_SET_TIMEOUT);
}

int percentage_to_loudness(int percentage) {
    printk(KERN_WARNING "percentage: %d\n", percentage);
    return values[percentage];
}
