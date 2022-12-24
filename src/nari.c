//
// Created by felix on 23.12.22.
//

#include "nari.h"
#include <linux/hid.h>
#include <linux/usb.h>

void mute_mic(struct device *dev){
    struct usb_interface *intf = to_usb_interface(dev->parent);
    struct usb_device *usbdev = interface_to_usbdev(intf);

    int foo = 0x0001;

    __u8 request = 1;
    __u8 requesttype = 0x21;
    __u16 value = 0x0100;
    __u16 index = 0x0300;
    //char *data = &foo;
    __u16 size = 1;
    int timeout = USB_CTRL_SET_TIMEOUT;

    usb_control_msg(usbdev, usb_sndctrlpipe(usbdev, 0), request, requesttype, value, index, &foo, size, timeout);
}

void unmute_mic(struct device *dev){
    struct usb_interface *intf = to_usb_interface(dev->parent);
    struct usb_device *usbdev = interface_to_usbdev(intf);

    int foo = 0x0000;

    __u8 request = 1;
    __u8 requesttype = 0x21;
    __u16 value = 0x0100;
    __u16 index = 0x0300;
    //char *data = &foo;
    __u16 size = 1;
    int timeout = USB_CTRL_SET_TIMEOUT;

    usb_control_msg(usbdev, usb_sndctrlpipe(usbdev, 0), request, requesttype, value, index, &foo, size, timeout);
}