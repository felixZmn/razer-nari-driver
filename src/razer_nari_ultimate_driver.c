//
// Created by felix on 07.11.22.
//

#include "../../razer-nari-driver/src/razer_nari_ultimate_driver.h"
#include <linux/module.h>
#include <linux/hid.h>
#include <linux/sysfs.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felix Zimmermann <felix.zimmermann.1.de@gmail.com>");
MODULE_VERSION("0.0.9");

// struct for device properties
struct myDevice {
    struct device *dev;
    long myParam;
};

static ssize_t get_value(struct device *dev, struct device_attribute *attr, char *buf){
    return sysfs_emit(buf, "%s\n", "foobar2000");
}

static ssize_t store_value(struct device *dev, struct device_attribute *attr, const char *buf, size_t count){
    // missbehaves: echo "foobar2000" > myParam results in
    // foobar2000
    // 00

    printk(KERN_ALERT "%s", buf);
    return sizeof(count);
}

static DEVICE_ATTR(myParam, S_IRUGO | S_IWUSR, get_value, store_value);

static struct attribute *nari_attrs[] = {
        &dev_attr_myParam.attr,
        NULL
};

static const struct attribute_group nari_attr_group = {
        .attrs = nari_attrs
};

/* newly inserted */
static int connect_razer_nari(struct hid_device *hdev, const struct hid_device_id *id) {
    // https://astro.uni-bonn.de/~sysstw/CompMan/ldp/lkmpg/node11.html
    struct myDevice *myDevice;
    int ret;

    printk(KERN_ALERT "razer nari connect called");

    /*register kernel space for later*/
    myDevice = devm_kzalloc(&hdev->dev, sizeof(*myDevice), GFP_KERNEL);
    if (!myDevice){
        return -ENOMEM;
    }
    myDevice->dev = &hdev->dev;

    hid_set_drvdata(hdev, myDevice); // ?!?!

    // create attribute groups
    ret = sysfs_create_group(&hdev->dev.kobj, &nari_attr_group);
    if (ret){
        printk(KERN_ALERT "echo: Cannot register sysfs attribute group\n");
    }
    return 0;
}

static void remove_razer_nari(struct hid_device *hdev) {
    sysfs_remove_group(&hdev->dev.kobj, &nari_attr_group);
    //kfree(&hdev->dev); // not needed, devm_kzalloc auto-frees memory
    printk(KERN_ALERT "razer nari disconnect called");
}

static const struct hid_device_id razer_nari_ultimate[] = {
    {HID_USB_DEVICE(USB_VENDOR_ID_RAZER, USB_DEVICE_RAZER_NARI_ULTIMATE)},
    {} /* terminating entry */
};

MODULE_DEVICE_TABLE(hid, razer_nari_ultimate);

static struct hid_driver razer_nari_driver = {
    .name = "razer_nari_ultimate",
    .id_table = razer_nari_ultimate,
    .probe = connect_razer_nari,
    .remove = remove_razer_nari,
};

module_hid_driver(razer_nari_driver);

/*
    struct usb_interface *intf = to_usb_interface(hdev->dev.parent);
    struct usb_device *usbdev = interface_to_usbdev(intf);

    __u8 request = 1;
    __u8 requesttype = 0x21;
    __u16 value = 0x0100;
    __u16 index = 0x0300;
    char *data = "00";
    __u16 size = 10;
    int timeout = USB_CTRL_SET_TIMEOUT;

    usb_control_msg(usbdev, usb_sndctrlpipe(usbdev, 0), request, requesttype, value, index, data, size, timeout);
*/