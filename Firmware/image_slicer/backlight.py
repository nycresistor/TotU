import pypruss
import time

data_addr = 0x00000000
period = 0x000000FF
gpio2_mask = 0x00003FC0
gpio3_mask = 0x003FC000

class BacklightController(object):
	data = [data_addr, period, gpio2_mask, gpio3_mask]
	bin_file = "./pario.bin"

	def __init__(self):
		for pwm in range(16):
			self.data.append(0x00000001) # Zero out the PWMs

		pypruss.modprobe() 			  				       	# This only has to be called once pr boot
		pypruss.init()										# Init the PRU
		pypruss.open(0)										# Open PRU event 0 which is PRU0_ARM_INTERRUPT
		pypruss.pruintc_init()								# Init the interrupt controller
		pypruss.pru_write_memory(0, 0, self.data)

	def run(self):
		pypruss.exec_program(0, self.bin_file)			# Load firmware "blinkled.bin" on PRU 0

	def update(self, channel, value):
		value += 1
		if value > 256:
			value = 256

		self.data[4 + channel] = value
		pypruss.pru_write_memory(0, 0, self.data)

	def stop(self):
		self.data[0] = 0x000000FF
		pypruss.pru_write_memory(0, 0, self.data)

if __name__ == "__main__":

	backlight = BacklightController()

	for _ in range(5):

		for val in range(0, 256):
			backlight.update(0, val)
			backlight.update(1, 256 - val)
			time.sleep(0.001)

		for val in range(0, 256):
			backlight.update(0, 256 - val)
			backlight.update(1, val)
			time.sleep(0.001)

	backlight.stop()

	pypruss.wait_for_event(0)	# Wait for event 0 which is connected to PRU0_ARM_INTERRUPT
	pypruss.clear_event(0)	# Clear the event
	pypruss.pru_disable(0)		# Disable PRU 0, this is already done by the firmware
	pypruss.exit()				# Exit, don't know what this does.