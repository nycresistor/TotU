all: \
	CAPE-BONE-TOTU-BK-00A0.dtbo \
	BB-BONE-LOGIMAX-00A0.dtbo

install:
	cp -v *.dtbo /lib/firmware
	
clean: 
	rm *.dtbo

%.dtbo : %.dts
	dtc -O dtb -o $@ -b 0 -@