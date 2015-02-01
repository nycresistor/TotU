all: \
	CAPE-BONE-TOTU-00A0.dtbo \
	BB-BONE-LOGIMAX-00A0.dtbo

install:
	cp -v *.dtbo /lib/firmware

active:	
	echo "CAPE-BONE-TOTU" > /sys/devices/bone_capemgr.9/slots
	echo "BB-BONE-LOGIMAX" > /sys/devices/bone_capemgr.9/slots

clean: 
	rm *.dtbo

%.dtbo : %.dts
	dtc -O dtb -o $@ -b 0 -@ $<
