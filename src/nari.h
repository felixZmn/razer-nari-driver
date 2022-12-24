//
// Created by felix on 23.12.22.
//


#ifndef RAZER_NARI_DRIVER_NARI_H
#define RAZER_NARI_DRIVER_NARI_H

#include <linux/usb.h>

void mute_mic(struct device *dev);
void unmute_mic(struct device *dev);

#endif //RAZER_NARI_DRIVER_NARI_H
