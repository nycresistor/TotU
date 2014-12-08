from PIL import Image
import sys, os

openFolder = sys.argv[1]
files = os.listdir(openFolder)

minImage = 0
maxImage = 0

images = []
ext = os.path.splitext(files[0])[1]

basenames = []
imageNames = {}
images = {}

def imageTo16BPP(image):
	sixteenBPP = []
	pixels = image.load()
	for y in range(image.size[1]):
		for x in range(image.size[0]):
			r = pixels[x, y][0] >> 3
			g = pixels[x, y][1] >> 2
			b = pixels[x, y][2] >> 3
			rgb = (r << 11) + (g << 5) + b
			
			hi, lo = split(rgb)

			hiBits, loBits = []
			for bit in range(8):
				hiBits.append((hi >> 7-bit) & 1 > 0)
				loBits.append((lo >> 7-bit) & 1 > 0)

			sixteenBPP.append(hiBits)
			sixteenBPP.append(loBits)
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

	# Compile dict of image numbers vs basenames
	for num in range(minImage, maxImage + 1):
		imageNames[num] = [basename for basename in basenames]

	# Open the images, get 16 bpp pixel list, append it to images dict
	for num, basenames in imageNames.items():
		images[num] = []
		for basename in basenames:
			im = Image.open(openFile).convert("RGB")
			sixteenBPP = imageTo16BPP(im)
			images[num].append(sixteenBPP)

	for num, pixelList in images.items():
		zippedPixels = [0 for 0 in (320 * 240 * 16 * 2)]
		numImages = len(pixelList)

		for byte8 in len(zippedPixels):
			for image in pixelList:
				for x in range(8)
					zippedPixels[byte8] <<= 1
					zippedPixels[byte8] & pixelList[byte8][x]

 # 	print("Length output %s" % len(zippedPixels))
	# with open(filebase + ".bin", "wb") as bin:
	# 	bin.write(zippedPixels)	




