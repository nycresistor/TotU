The BBB won't boot to the SD card on its own, you have to hold down the boot switch underneath the SD card while plugging in the USB cable ([https://i.imgur.com/3EXBSue.png](https://i.imgur.com/3EXBSue.png))

Plug a screen into port A1, the top left port of the A bank. You can also plug a second screen into A8, the bottom left port of the A bank. I didn't test any of the other ports but they should also work.

`> ssh ubuntu@192.168.7.2`

(password is temppwd)

`> sudo su -`

(password is temppwd)

`> cd TotU/Firmware/dts`
`> make active`

`> cd ../backlight_controller`
`> ./setup.sh`

`> python backlight.py`

(backlight should pulse 5 times slowly)

`> cd ../tft_controller`

`> ./totu ../image_slicer/data/test/bin`

(I don't remember if that's the exact path, but it should be the path to the folder with the .bin files)

After a few seconds the image should appear on the screen. The backlight will need to be on, so you might want to open a separate ssh session to run the backlight.py script while you run tft_controller/totu.

To set the backlight for A1 on full:

(assuming you've already run backlight_controller/setup.sh)

`> echo 1 > /sys/class/gpio/gpio7/value`

To set the backlight for A8 on full:

(assuming you've already run backlight_controller/setup.sh)

`> echo 1 > /sys/class/gpio/gpio20/value`

If something doesn't work, check `> dmesg` and/or reboot and try again.
