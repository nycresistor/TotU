import pypruss
import time
import mmap
import struct

data_addr = 0x00000001
period = 0x000000FF
gpio0_mask = 0x0010FF9C
gpio1_mask = 0x00020000
gpio2_mask = 0x0003FFC0
gpio3_mask = 0x002BC000

class BacklightController(object):
	data = [data_addr, period, gpio0_mask, gpio1_mask, gpio2_mask, gpio3_mask]
	bin_file = "./pario.bin"

	def __init__(self):
		for pwm in range(32):
			self.data.append(0x00000001) # Zero out the PWMs

		pypruss.modprobe() 			  				       	# This only has to be called once pr boot
		pypruss.init()										# Init the PRU
		pypruss.open(0)										# Open PRU event 0 which is PRU0_ARM_INTERRUPT
		pypruss.pruintc_init()								# Init the interrupt controller
		pypruss.pru_write_memory(0, 0, self.data)

	def run(self):
		print("Execing %s" % self.bin_file)
		pypruss.exec_program(0, self.bin_file)			# Load firmware "blinkled.bin" on PRU 0

	def update(self, channel, value):
		value += 1
		if value > 256:
			value = 256

		self.data[6 + channel] = value
		# print(self.data)
		pypruss.pru_write_memory(0, 0, self.data)

	def stop(self):
		print("Ending")
		self.data[0] = 0x000000FF
		pypruss.pru_write_memory(0, 0, self.data)

if __name__ == "__main__":

	backlight = BacklightController()
	backlight.run()

	for _ in range(5):

		for val in range(0, 256):
			backlight.update(0, val)
			backlight.update(1, 256 - val)
			backlight.update(2, val)
			backlight.update(3, 256 - val)
			backlight.update(4, val)
			backlight.update(5, 256 - val)
			backlight.update(6, val)
			backlight.update(7, 256 - val)
			time.sleep(0.001)

		for val in range(0, 256):
			backlight.update(0, 256 - val)
			backlight.update(1, val)
			backlight.update(2, 256 - val)
			backlight.update(3, val)
			backlight.update(4, 256 - val)
			backlight.update(5, val)
			backlight.update(6, 256 - val)
			backlight.update(7, val)
			time.sleep(0.001)

	# backlight.update(0, 255)
	# backlight.update(1, 254)

	# time.sleep(1)

	
	# PRU_ICSS = 0x4A300000 
	# PRU_ICSS_LEN = 512*1024

	# RAM0_START = 0x00000000
	# RAM1_START = 0x00002000
	# RAM2_START = 0x00012000




	
	backlight.stop()

	pypruss.wait_for_event(0)	# Wait for event 0 which is connected to PRU0_ARM_INTERRUPT
	pypruss.clear_event(0)	# Clear the event
	pypruss.pru_disable(0)		# Disable PRU 0, this is already done by the firmware
	pypruss.exit()				# Exit, don't know what this does.

	# with open("/dev/mem", "r+b") as f:	       
	# 	ddr_mem = mmap.mmap(f.fileno(), PRU_ICSS_LEN, offset=PRU_ICSS) 
	# 	local = struct.unpack('L', ddr_mem[RAM0_START:RAM0_START+4])
	# 	print hex(local[0])
	# 	local = struct.unpack('L', ddr_mem[RAM0_START+4:RAM0_START+8])
	# 	print hex(local[0])

