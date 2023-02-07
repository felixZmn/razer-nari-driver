//
// Created by felix on 07.11.22.
//

#include "driver.h"
#include "nari.h"
#include <linux/module.h>
#include <linux/hid.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felix Zimmermann <felix.zimmermann.1.de@gmail.com>");
MODULE_VERSION("0.0.9");

// struct for device properties
struct myDevice {
    struct device *dev;
    long myParam;
    int mute;
};

struct myDevice *headset;

static ssize_t read_mute_state(struct device *dev, struct device_attribute *attr, char *buf) {
    return sysfs_emit(buf, "%d\n", headset->mute); // returns always 0
}

static ssize_t set_mute_state(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    if (*buf == '0') {
        headset->mute = 0;
        unmute_mic(dev);
    } else if (*buf == '1') {
        headset->mute = 1;
        mute_mic(dev);
    }
    return sizeof(count);
}

static ssize_t get_value(struct device *dev, struct device_attribute *attr, char *buf) {
    return sysfs_emit(buf, "%s\n", "foobar2000");
}

static ssize_t store_value(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    printk(KERN_ALERT "%s", buf);
    return sizeof(count);
}

static DEVICE_ATTR(mute, S_IRUGO | S_IWUSR, read_mute_state, set_mute_state);
static DEVICE_ATTR(myParam, S_IRUGO | S_IWUSR, get_value, store_value);

static struct attribute *nari_attrs[] = {&dev_attr_myParam.attr, &dev_attr_mute.attr, NULL};

static const struct attribute_group nari_attr_group = {.attrs = nari_attrs,};

/* newly inserted */
static int connect_razer_nari(struct hid_device *hdev, const struct hid_device_id *id) {
    int ret;
    /*register kernel space for later*/
    headset = devm_kzalloc(&hdev->dev, sizeof(*headset), GFP_KERNEL);
    if (!headset) {
        return -ENOMEM;
    }
    headset->dev = &hdev->dev;

    // create attribute groups
    ret = sysfs_create_group(&hdev->dev.kobj, &nari_attr_group);
    if (ret) {
        printk(KERN_WARNING "echo: Cannot register sysfs attribute group\n");
    }
    return 0;
}

static void remove_razer_nari(struct hid_device *hdev) {
    sysfs_remove_group(&hdev->dev.kobj, &nari_attr_group);
}

static const struct hid_device_id razer_nari_ultimate[] = {{HID_USB_DEVICE(USB_VENDOR_ID_RAZER,
                                                                           USB_DEVICE_RAZER_NARI_ULTIMATE)},
                                                           {} /* terminating entry */
};

MODULE_DEVICE_TABLE(hid, razer_nari_ultimate);

static struct hid_driver razer_nari_driver = {.name = "razer_nari_ultimate", .id_table = razer_nari_ultimate, .probe = connect_razer_nari, .remove = remove_razer_nari,};

module_hid_driver(razer_nari_driver);
