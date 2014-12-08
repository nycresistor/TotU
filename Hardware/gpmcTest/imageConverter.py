from PIL import Image
import sys, os

openFile = sys.argv[1]
filename = os.path.split(openFile)[1]
filebase = os.path.splitext(filename)[0]

myImage = Image.open(openFile).convert("RGB")
pixels = myImage.load()

sixteenBPP = []
frame = bytearray()
output = bytearray()

for y in range(myImage.size[1]):
	for x in range(myImage.size[0]):
		r = pixels[x, y][0] >> 3
		g = pixels[x, y][1] >> 2
		b = pixels[x, y][2] >> 3
		out = (r << 11) + (g << 5) + b
		sixteenBPP.append(out)

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

print("Length sixteenBPP %s" % len(sixteenBPP))
for byte in sixteenBPP:
	for i in range(16):
		if (byte >> 15-i) & 1 > 0:
			output.append(255)
			output.append(255)
		else:
			output.append(0)
			output.append(0)

print("Length output %s" % len(output))
with open(filebase + ".bin", "wb") as bin:
	bin.write(output)	


