import pypruss
import time

# Must be != 0 to start, FF to stop
command = 0x00000001

# 255 seems to work as our PWM period
count = 0x000000FF

# These are masks of our used GPIO pins
# so that we don't screw with pins we're
# not using for the backlights
gpio0_mask = 0x0010FF9C
gpio1_mask = 0x00020000
# gpio1_mask = 0xA0020000
gpio2_mask = 0x0003FFC0
# gpio2_mask = 0x0143FFC0
gpio3_mask = 0x002BC000

class BacklightController(object):
	data = [command, count, gpio0_mask, gpio1_mask, gpio2_mask, gpio3_mask]
	bin_file = "./totu.bin"

	def __init__(self):
		for pwm in range(32):
			# Initialize the 32 PWM channels to 1
			self.data.append(0x00000001)

		# Init the PRU
		pypruss.modprobe()
		pypruss.init()
		# Open PRU event 0 (PRU0_ARM_INTERRUPT)
		pypruss.open(0)
		# Init the interrupt controller
		pypruss.pruintc_init()
		# Write out our data structure to the PRU Data Ram
		pypruss.pru_write_memory(0, 0, self.data)

	def run(self):
		print("Starting PRU binary %s" % self.bin_file)
		pypruss.exec_program(0, self.bin_file)

	def update(self, channel, value):
		# Clamp the value to 255
		# this would have to change if we change our period
		value += 1
		if value > 256:
			value = 256

		# Channels start after six command bytes
		self.data[6 + channel] = value
		# print(self.data)
		# Write our whole data structure out to PRU Data Ram
		pypruss.pru_write_memory(0, 0, self.data)

	def stop(self):
		print("Ending")
		# Set our command byte to FF and write it out
		self.data[0] = 0x000000FF
		pypruss.pru_write_memory(0, 0, self.data)

		# Wait for event 0 which is connected to PRU0_ARM_INTERRUPT
		pypruss.wait_for_event(0)
		# Clear the event
		pypruss.clear_event(0)
		# Disable PRU 0, this is already done by the firmware
		pypruss.pru_disable(0)
		# Exit, don't know what this does.
		pypruss.exit()

if __name__ == "__main__":

	backlights = BacklightController()
	backlights.run()

	for _ in range(5):

		for val in range(0, 256):
			for channel in range(32):
				backlights.update(channel, val)
			time.sleep(0.001)

		time.sleep(0.5)

		for val in range(0, 256):
			for channel in range(32):
				backlights.update(channel, 256 - val)
			time.sleep(0.001)

		time.sleep(0.5)
	
	backlights.stop()
