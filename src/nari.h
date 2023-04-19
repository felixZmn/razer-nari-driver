//
// Created by felix on 23.12.22.
//

#ifndef RAZER_NARI_DRIVER_NARI_H
#define RAZER_NARI_DRIVER_NARI_H

#include <linux/usb.h>


void set_mute(struct device *dev, int state);

#endif // RAZER_NARI_DRIVER_NARI_H
