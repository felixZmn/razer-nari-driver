//
// Created by felix on 23.12.22.
//

#include "nari.h"
#include <linux/hid.h>
#include <linux/usb.h>

/**
 * set active state of the headset microphone
 * @param dev pointer to the device
 * @param state off/muted: 0x0001; on/unmuted: 0x0000;
 */
void set_mute(struct device *dev, int state) {
    if(state != 0x0000 && state != 0x0001){
        // wrong values provided, do nothing
        return;
    }
    struct usb_interface *intf = to_usb_interface(dev->parent);
    struct usb_device *usbdev = interface_to_usbdev(intf);
    usb_control_msg(usbdev, usb_sndctrlpipe(usbdev, 0), 1, 0x21, 0x0100, 0x0300, &state, 1, USB_CTRL_SET_TIMEOUT);
}
