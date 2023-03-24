//
// Created by felix on 07.11.22.
//

#include "driver.h"
#include "nari.h"
#include <linux/module.h>
#include <linux/hid.h>
#include <linux/sysfs.h>
#include <linux/usb.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felix Zimmermann <felix.zimmermann.1.de@gmail.com>");
MODULE_VERSION("0.0.9");

// struct for device properties
struct razerNari {
    struct usb_interface *interface;
    struct urb *urb;
    struct usb_device *udev;
    struct usb_endpoint_descriptor *int_in_endpoint;
    int buffer_size;
    long myParam;
    int mute;
};

struct razerNari *nari;

static ssize_t read_mute_state(struct device *dev, struct device_attribute *attr, char *buf) {
    return sysfs_emit(buf, "%d\n", nari->mute); // returns always 0
}

static ssize_t set_mute_state(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    if (*buf == '0') {
        nari->mute = 0;
        unmute_mic(dev);
    } else if (*buf == '1') {
        nari->mute = 1;
        mute_mic(dev);
    }
    return sizeof(count);
}

static ssize_t get_value(struct device *dev, struct device_attribute *attr, char *buf) {
    return sysfs_emit(buf, "%s\n", "foobar2000");
}

static ssize_t store_value(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    printk(KERN_DEFAULT "%s", buf);
    return sizeof(count);
}

static DEVICE_ATTR(mute, S_IRUGO | S_IWUSR, read_mute_state, set_mute_state);
static DEVICE_ATTR(myParam, S_IRUGO | S_IWUSR, get_value, store_value);

static struct attribute *nari_attrs[] = {&dev_attr_myParam.attr, &dev_attr_mute.attr, NULL};

static const struct attribute_group nari_attr_group = {.attrs = nari_attrs,};

void complete_nari_urb(struct urb *urb) {
    int *data;
    data = urb->transfer_buffer;

    switch (data[0]) {
        case 101:
            printk(KERN_DEFAULT "mic muted");
            break;
        case 1125:
            printk("mic active");
            break;
//        case 356:
//            printk("nari connected to dongle");
//            break;
//        case 868:
//            printk("nari switched off");
//            break;
        default:
            printk("%d\n", data[0]);
            break;
    }

    usb_submit_urb(urb, GFP_KERNEL);
}

static int create_interrupt_urb(struct razerNari *dev) {
    void *int_buffer;
    dev->urb = usb_alloc_urb(0, GFP_KERNEL);
    int_buffer = kmalloc(dev->buffer_size, GFP_KERNEL);
    usb_fill_int_urb(dev->urb, dev->udev, usb_rcvintpipe(dev->udev, dev->int_in_endpoint->bEndpointAddress), int_buffer,
                     dev->buffer_size, complete_nari_urb, dev, 5);
    return usb_submit_urb(dev->urb, GFP_KERNEL);
}

/* newly inserted */
static int probe_nari(struct hid_device *hdev, const struct hid_device_id *id) {
    /*register kernel space for later*/
    nari = devm_kzalloc(&hdev->dev, sizeof(*nari), GFP_KERNEL);

    if (!nari) {
        return -ENOMEM;
    }
    nari->buffer_size = 2; // how many bytes
    nari->udev = to_usb_device(hdev->dev.parent->parent);
    nari->interface = to_usb_interface(hdev->dev.parent);
    if (usb_find_common_endpoints_reverse(nari->interface->cur_altsetting, NULL, NULL, &nari->int_in_endpoint, NULL)) {
        // error
        return 0;
    }
    // create attribute groups
    if (sysfs_create_group(&hdev->dev.kobj, &nari_attr_group)) {
        printk(KERN_DEFAULT "Cannot register sysfs attribute group\n");
    }

    create_interrupt_urb(nari);

    return 0;
}


static void remove_nari(struct hid_device *hdev) {
    sysfs_remove_group(&hdev->dev.kobj, &nari_attr_group);
}

static const struct hid_device_id razer_nari_ultimate[] = {{HID_USB_DEVICE(USB_VENDOR_ID_RAZER,
                                                                           USB_DEVICE_RAZER_NARI_ULTIMATE)},
                                                           {} /* terminating entry */
};

MODULE_DEVICE_TABLE(hid, razer_nari_ultimate);

static struct hid_driver razer_nari_driver = {.name = "razer_nari_ultimate", .id_table = razer_nari_ultimate, .probe = probe_nari, .remove = remove_nari,};

module_hid_driver(razer_nari_driver);
