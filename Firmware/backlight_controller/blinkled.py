''' blinkled.py - test script for the PyPRUSS library
It blinks the user leds ten times'''

import pypruss

gpio2_mask = '\x00\x00\x3f\xc0'
gpio3_mask = '\x00\x3f\xc0\x00'

data = [0x00000000, 0x000000FF, 0x00003FC0, 0x003FC000]

for _ in range(16):
	data.append(0x00000001)

pypruss.modprobe() 			  				       	# This only has to be called once pr boot
pypruss.init()										# Init the PRU
pypruss.open(0)										# Open PRU event 0 which is PRU0_ARM_INTERRUPT
pypruss.pruintc_init()								# Init the interrupt controller
pypruss.exec_program(0, "./pario.bin")			# Load firmware "blinkled.bin" on PRU 0

pypruss.pru_write_memory(0, 0, data)

import time
for x in range(256):
	data[4] = 0x00000000 + x
	pypruss.pru_write_memory(0,0, data)
	time.sleep(0.001)

for x in xrange(255, 0, -1):
	data[4] = 0x00000000 + x
	pypruss.pru_write_memory(0, 0, data)
	time.sleep(0.001)


pypruss.wait_for_event(0)							# Wait for event 0 which is connected to PRU0_ARM_INTERRUPT
pypruss.clear_event(0)								# Clear the event
pypruss.pru_disable(0)								# Disable PRU 0, this is already done by the firmware
pypruss.exit()										# Exit, don't know what this does. 
