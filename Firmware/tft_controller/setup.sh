# Export something on GPIO 2 to turn it on?
echo 65 > /sys/class/gpio/export
echo 20 > /sys/class/gpio/export
echo high > /sys/class/gpio/gpio20/direction
