---
layout: post
title: Beaglebone Black - GPMC part 1
author: Max
---

So you've got yourself a [Beaglebone Black](http://beagleboard.org/BLACK). What's this GPMC thing?

GPMC stands for General Purpose Memory Controller. But don't let the name fool you, it's good for much more than just interfacing with memory systems. The GPMC is a 16-bit wide data bus with address pins that can be either multiplexed (on the data bus) or non-multiplexed (separate pins). The GPMC has several modes that can be used to access SRAM, Flash memory, or other perhiperals. If you think about it as a 16-bit wide arbitrary waveform generator, a whole world of possibilities opens up for you. Using some basic C code and a kernal module, you can communicate with devices at 50 MHz or more, over ten times faster than bit-banging the Beaglebone's GPIO pins directly. For our project, the [Temple of the Unticketed](http://templeoftheunticketed.net), we're using the GPMC bus as a 16-bit wide, 50 MHz parallel SPI master to drive 32 TFT screens.
<!--more-->
***

#### Technical Details

The GPMC bus is comprised of the following signals:

| Signal   | Description |
| ------   | ----------- |
| CSxn	   | The bus provides three chip selects (0 to 2)
| ADVn	   | This signal when asserted low indicate that the bus is used for addressing (multiplexed mode)
| AD[15:0] | The sixteen bit multiplexed address/data bus (multiplexed mode) or data-only bus (non-multiplexed mode)
| WEn	   | Write enable signal. Indicates an ongoing write when asserted low.
| OEn	   | Output Enable signal. Indicates an ongoing read when asserted low. The bus is high impedance when OEn is low to allow the slave to take control of the data bus
| BE[1:0]  | Byte eanble signal. These two signals allow to indicate high or low byte access of the 16 bit data when doing writes
| CLK	   | GPMC external clock. The GPMC bus can provide its own clock with speeds of up to 100MHz when using the synchronous mode
| A[27:0]  | Non-multiplexed address outputs

<br/>

There are a few other pins on the GPMC bus but these are the most important.

The GPMC can operate at either 8-bits or 16-bits wide. It can operate as multiplexed (data and address use the same pins, multiplexed in the time domain), or non-multiplexed (address pins are separate, data pins only carry data). It can also run in either synchronous mode - in which it provides a clock signal - or asynchronous mode. It can also address up to three separate devices (using chip selects) and has a wide operating frequency range - up to 100 MHz when addressing a single device.

*Well that's just dandy, how do I configure this GPMC bus?* Well, that's the sticker. Some time ago, the Beaglebone project moved to configuring everything with Linux [Device Tree](http://www.devicetree.org/Main_Page). Device Tree would be just peachy, if the implementation weren't completely borked. Luckly, with no small amount of help from [Trammell Hudson](https://trmm.net/) and the [ValentFX LOGI Bone](http://valentfx.com/logi-bone/) project, we've been able to come up with some working Device Tree overlays. 

***

#### Next Steps

In our next installment, I'll cover getting our Beaglebone Black set up to get the GPMC running, a "Hello World" tutorial if you will. Stay tuned!

***

#### Further Reading

* Some sparse but helpful information can be found on the [ValentFX (LOGI Bone) Wiki](http://valentfx.com/wiki/index.php?title=LOGI_-_BBB_GPMC_Bus-_HW).
* Much can be learned from the large but surprisingly readable [Technical Reference Manual](http://www.ti.com/lit/ug/spruh73k/spruh73k.pdf) for the TI AM335x processor (the Beaglebone Black CPU).
* The [Hipster Circuits blog](http://hipstercircuits.com/category/device-tree/) has a lot of great posts, not specifically about the GPMC, but there is some great info about Device Tree, the PRU system, and the Beaglebone Black in general.
