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

// enum to collect usb events
enum interruptEvents {
    MIC_ENABLED = 101,
    MIC_DISABLED = 1125,
    CONNECTED = 356,
    DISCONNECTED = 868,
};

// struct for microphone properties
struct mic {
    int volume;
    int active;
};

// struct for device properties
struct razerNari {
    struct usb_interface *interface;
    struct urb *urb;
    struct usb_device *udev;
    struct usb_endpoint_descriptor *int_in_endpoint;
    struct mic mic;
    int buffer_size;
    long myParam;
};

struct razerNari *nari;

/**
 * Read value from driver and provide to sysfs
 */
static ssize_t read_mic_state(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sysfs_emit(buf, "%d\n", nari->mic.active);
}

/**
 * write mic state from sysfs to driver
 */
static ssize_t write_mic_state(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    if (*buf == '0')
    {
        // deactivate mic
        nari->mic.active = 1;
        set_mute(dev, 0x01);
    }
    else if (*buf == '1')
    {
        // activate mic
        nari->mic.active = 0;
        set_mute(dev, 0x00);
    }
    return sizeof(count);
}

// sysfs sample
static ssize_t read_mic_volume(struct device *dev, struct device_attribute *attr, char *buf)
{
    return sysfs_emit(buf, "%d\n", nari->mic.volume);
}

// sysfs sample
static ssize_t write_mic_volume(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int percentage;
    if (kstrtoint(buf, 10, &percentage) != 0) {
        // error
        return sizeof(count);
    }
    set_mic_volume(dev, percentage);
    return sizeof(count);
}

static DEVICE_ATTR(active, S_IRUGO | S_IWUSR, read_mic_state, write_mic_state);
static DEVICE_ATTR(myParam, S_IRUGO | S_IWUSR, read_mic_volume, write_mic_volume);

static struct attribute *nari_attrs[] = {&dev_attr_myParam.attr, &dev_attr_active.attr, NULL};

static const struct attribute_group nari_attr_group = {.attrs = nari_attrs,};

/**
 * Interrupt handler
 * @param urb contains interrupt data
 */
void complete_nari_urb(struct urb *urb) {
    int *data;
    data = urb->transfer_buffer;
    switch (data[0]) {
        case MIC_ENABLED:
            // activate mic
            nari->mic.active = 1;
            break;
        case MIC_DISABLED:
            // deactivate mic
            nari->mic.active = 0;
            break;
        case CONNECTED:
            // wireless connection between dongle and headset established
            break;
        case DISCONNECTED:
            // wireless connection between dongle and headset lost
            break;
        default:
            printk("%d\n", data[0]);
            break;
    }

    // resubmit urb to catch next interrupt
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
    pr_info("initializing nari...\n");
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
        printk(KERN_DEFAULT "Cannot register sysfs attribute group\n"); // should driver exit?
    }

    create_interrupt_urb(nari);

    pr_info("nari initialized\n");
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
