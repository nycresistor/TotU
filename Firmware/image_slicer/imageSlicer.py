from PIL import Image
import sys, os, time
import numpy
# import convert

openFolder = sys.argv[1]
files = os.listdir(openFolder)

minImage = 1
maxImage = 0

images = []
ext = os.path.splitext(files[0])[1]

basenames = []
imageNames = {}
images = {}

def pixelsToBytes(pixels, width, height):
	out = []
	for y in range(height):
		for x in range(width):
			out.append((pixels[x, y][0] & 0xF8) + (pixels[x, y][1] >> 5))
			out.append(((pixels[x, y][1] & 0x1C) << 3) + ((pixels[x, y][2] & 0xF8) >> 3))
	return out

def bytesToBits(bytes):
	a = numpy.array(bytes, dtype=numpy.uint8)
	up = numpy.unpackbits(a)
	return up

def rotate(data):
	return list(zip(*data[::-1]))

def getImageData(frameNum):
	imageData = []
	startGID = time.time()
	for basename in basenames:
		startBN = time.time()
		openFile = os.path.join(openFolder, "%s-%s%s" % (basename, frameNum, ext))
		image = Image.open(openFile).convert("RGB")
		print("\tLoading image %s (%s, %s)" % (openFile, image.size[0], image.size[1]))
		
		bytes = pixelsToBytes(image.load(), image.size[0], image.size[1])
		bits = bytesToBits(bytes)
		
		imageData.append(bits)
		print("\tTook %.2f seconds to open and convert image" % (time.time() - startBN))
	print("\tTook %.2f seconds to get all image data" % (time.time() - startGID))

	return imageData

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


		rotated = rotate(imageData)
		imageData = None
		# unpacked = numpy.unpackbits(imageData, axis = 1)

		# # print unpacked

		# stacked = numpy.column_stack(unpacked)

		# # print stacked

		packed = numpy.packbits(rotated, axis = 1)

		# print packed

		for byte in packed:
			outputBytes.append(byte[1])
			outputBytes.append(byte[0])		

		took = time.time() - start
		print("Frame %s output size %s took %s seconds" % (frame, len(outputBytes), took))

		with open(os.path.join("seq", "seq-%s.bin" % frame), 'wb') as bin:
			bin.write(outputBytes)
