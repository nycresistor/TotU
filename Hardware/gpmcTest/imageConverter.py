from PIL import Image
import sys, os

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
	print("Loading image (%s, %s)" % (image.size[0], image.size[1]))
	for y in range(image.size[1]):
		for x in range(image.size[0]):
			r = pixels[x, y][0] >> 3
			g = pixels[x, y][1] >> 2
			b = pixels[x, y][2] >> 3
			rgb = (r << 11) + (g << 5) + b
			
			hi, lo = split(rgb)

			hiBits, loBits = [], []
			for bit in range(8):
				hiBits.append((hi >> 7-bit) & 1 > 0)
				loBits.append((lo >> 7-bit) & 1 > 0)

			sixteenBPP.append(hiBits)
			sixteenBPP.append(loBits)

	print("Returning %s 8-bit bytes" % len(sixteenBPP))
	return sixteenBPP

def split(byte16):
	hi = byte16 >> 8
	lo = byte16 & 255
	return hi, lo

frame = bytearray()
output = bytearray()


# for x in range(320 * 240 * 8):
# 	if x % 2 == 0:
# 		sixteenBPP.append(65535)
# 		sixteenBPP.append(65535)
# 	else:
# 		sixteenBPP.append(0)
# 		sixteenBPP.append(0)

# print("Length sixteenBPP %s" % len(sixteenBPP))
# for byte in sixteenBPP:
# 	hi = byte >> 8
# 	lo = byte & 255
# 	frame.append(hi)
# 	frame.append(lo)

# print("Length sixteenBPP %s" % len(sixteenBPP))
# for byte in sixteenBPP:
# 	for i in range(16):
# 		if (byte >> 15-i) & 1 > 0:
# 			output.append(255)
# 			output.append(255)
# 		else:
# 			output.append(0)
# 			output.append(0)

#

def getByte(frameNum, byteNum):
	output = []
	imageData = images[frameNum]
	for base in range(len(basenames)):
		output.append(imageData[base][byteNum])
	return output


if __name__ == "__main__":

	# Get basenames, min and max image numbers (format: image-0.bmp)
	for filename in files:
		filename = os.path.splitext(filename)[0]
		num = int(filename.split('-')[1])
		minImage = min(minImage, num)
		maxImage = max(maxImage, num)

		basename = filename.split('-')[0]
		if basename not in basenames:
			basenames.append(basename)

	print("Min: %s Max: %s" % (minImage, maxImage))

	# Compile dict of image numbers vs basenames
	for num in range(minImage, maxImage + 1):
		imageNames[num] = [basename for basename in basenames]

	print(imageNames)

	# Open the images, get 16 bpp pixel list, append it to images dict
	for num, basenames in imageNames.items():
		images[num] = []
		for basename in basenames:
			openFile = os.path.join(openFolder, "%s-%s%s" % (basename, num, ext))
			print openFile
			im = Image.open(openFile).convert("RGB")
			sixteenBPP = imageTo16BPP(im)
			images[num].append(sixteenBPP)

	outputBytes = []

	# Handle all frames
	for frame in range(minImage, maxImage + 1):

		print("Starting frame %s" % frame)
		explodedBytes = []

		# All bytes in the output array
		for byteNum in range(153600):

			# get the x byte from each image
			bytes = getByte(frame, byteNum)
			
			for i in range(8):
				explodedBytes.append([byte[i] for byte in bytes])			

		outputBytes = bytearray()
		for byte in explodedBytes:
			num = 0
			for bit in byte:
				num <<= 1
				num |= int(bit)

				hi, lo = split(num)
			outputBytes.append(lo)	
			outputBytes.append(hi)

		print("Frame %s output size %s" % (frame, len(outputBytes)))

		with open(os.path.join("seq", "seq-%s.bin" % frame), 'wb') as bin:
			bin.write(outputBytes)