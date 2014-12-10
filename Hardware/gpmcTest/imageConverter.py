from PIL import Image
import sys, os, time
import numpy
# import threading, Queue

openFolder = sys.argv[1]
files = os.listdir(openFolder)

minImage = 1
maxImage = 0

images = []
ext = os.path.splitext(files[0])[1]

basenames = []
imageNames = {}
images = {}



def imageTo16BPP(image):
	sixteenBPP = []
	pixels = image.load()
	start = time.time()
	for y in range(image.size[1]):
		for x in range(image.size[0]):

			# r = pixels[x, y][0] >> 3
			# g = pixels[x, y][1] >> 2
			# b = pixels[x, y][2] >> 3
			# rgb = (r << 11) + (g << 5) + b

			# b1 = ((pixels[x, y][0] >> 3) << 3) + (pixels[x, y][1] >> 5)
			# b2 = ((pixels[x, y][1] >> 3) << 3) + (pixels[x, y][2] >> 3)

			b1 = (pixels[x, y][0] & 0xF8) + (pixels[x, y][1] & 0x07)
			b2 = (pixels[x, y][1] & 0xE0) + (pixels[x, y][2] & 0x1F)
			
			# hi, lo = split(rgb)
			# hiBits, loBits = [], []
			# for bit in range(8):
			# 	# hiBits.append((hi >> 7-bit) & 1 > 0)
			# 	# loBits.append((lo >> 7-bit) & 1 > 0)
			# 	hiBits.append((b1 >> 7-bit) & 1 > 0)
			# 	loBits.append((b2 >> 7-bit) & 1 > 0)

			a = numpy.array([[b1], [b2]], dtype=numpy.uint8)
			up = numpy.unpackbits(a, axis=1)

			# sixteenBPP.append(hiBits)
			# sixteenBPP.append(loBits)

			sixteenBPP.append(up[1])
			sixteenBPP.append(up[0])



	# print("Returning %s 8-bit bytes" % len(sixteenBPP))
	print("\tTook %.2f seconds to convert image to 16BPP" % (time.time() - start))
	return sixteenBPP

def split(byte16):
	hi = byte16 >> 8
	lo = byte16 & 255
	return hi, lo

frame = bytearray()
output = bytearray()

def getByte(imageData, frameNum, byteNum):
	output = []
	for base in range(len(basenames)):
		output.append(imageData[base][byteNum])
	return output

def getImageData(frameNum):
	imageData = []
	startGID = time.time()
	for basename in basenames:
		startBN = time.time()
		openFile = os.path.join(openFolder, "%s-%s%s" % (basename, frameNum, ext))
		image = Image.open(openFile).convert("RGB")
		print("\tLoading image %s (%s, %s)" % (openFile, image.size[0], image.size[1]))
		sixteenBPP = imageTo16BPP(image)
		imageData.append(sixteenBPP)
		# imageData.extend(sixteenBPP)
		print("\tTook %.2f seconds to open and convert image" % (time.time() - startBN))
	print("\tTook %.2f seconds to get all image data" % (time.time() - startGID))

	out = numpy.array(imageData, dtype=numpy.uint8)
	return out

if __name__ == "__main__":
	print("\n\n")

	# Get basenames, min and max image numbers (format: image-0.bmp)
	for filename in files:
		print(filename)
		filename = os.path.splitext(filename)[0]
		num = int(filename.split('-')[1])
		minImage = min(minImage, num)
		maxImage = max(maxImage, num)

		basename = filename.split('-')[0]
		if basename not in basenames:
			basenames.append(basename)

	# Compile dict of image numbers vs basenames
	for num in range(minImage, maxImage + 1):
		imageNames[num] = [basename for basename in basenames]

	outputBytes = []

	# Handle all frames
	for frame in range(minImage, maxImage + 1):

		print("Starting frame %s" % frame)
		start = time.time()
		explodedBytes = []
		imageData = getImageData(frame)
		outputBytes = bytearray()

		print("image data %s %s" % (len(imageData), len(imageData[0])))

		unpacked = numpy.unpackbits(imageData, axis = 1)

		# print unpacked

		stacked = numpy.column_stack(unpacked)

		# print stacked

		packed = numpy.packbits(stacked, axis = 1)

		# print packed

		for byte in packed:
			outputBytes.append(byte[1])
			outputBytes.append(byte[0])

		


		# for byteNum in xrange(153600):
		# 	bytes = []
		# 	for q in range(len(basenames)):
		# 		bytes.append(imageData[byteNum + (153600 * q)])

		# 	for i in range(8):
		# 		for byte in [[byte[i] for byte in bytes]]:
		# 			num = 0
		# 			for bit in byte:
		# 				num <<= 1
		# 				num |= int(bit)

		# 			hi, lo = split(num)
		# 			outputBytes.append(lo)	
		# 			outputBytes.append(hi)


		# All bytes in the output array
		# for byteNum in xrange(153600):

		# 	# get the x byte from each image
		# 	bytes = getByte(imageData, frame, byteNum)
		
		# 	for i in range(8):
		# 		for byte in [[byte[i] for byte in bytes]]:
		# 			num = 0
		# 			for bit in byte:
		# 				num <<= 1
		# 				num |= int(bit)

		# 			hi, lo = split(num)
		# 			outputBytes.append(lo)	
		# 			outputBytes.append(hi)			

		took = time.time() - start
		print("Frame %s output size %s took %s seconds" % (frame, len(outputBytes), took))

		with open(os.path.join("seq", "seq-%s.bin" % frame), 'wb') as bin:
			bin.write(outputBytes)