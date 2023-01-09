# Developing notes

## installing/uninstalling driver
--- replaced by makefile ---

## Install wireshark - ubuntu
[Guide](https://wiki.wireshark.org/CaptureSetup/USB)
### tl;dr:
`sudo apt install wireshark libpcap0.8`

## Capture USB traffic
1. Enable usbmon kernel module:\
`sudo modprobe usbmon`
2. Launch wireshark - depending on the choosen setup, sudo could be needed
3. choose the right interfance.\
![img.png](images/usb%20interfaces.png)
4. Happy capturing

Note: To capture the traffic between the original driver and the Headset, you have to install wireshark onthe same Windows machine as the driver or install Windows and razer driver inside a VM and pass the dongle to it. 