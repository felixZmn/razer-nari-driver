//
// Created by felix on 23.12.22.
//

#ifndef RAZER_NARI_DRIVER_NARI_H
#define RAZER_NARI_DRIVER_NARI_H

#include <linux/usb.h>


void set_mute(struct device *dev, int state);

void set_mic_volume(struct device *dev, int percentage);

int percentage_to_loudness(int percentage);

#endif // RAZER_NARI_DRIVER_NARI_H
