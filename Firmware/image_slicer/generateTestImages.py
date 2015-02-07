from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
import shutil

convert_cmds = []

for bank in ["A", "B"]:

	for frameNumber in range(30):

		for screenNumber in range(16):

			im = Image.new("RGB", (320, 240), "black")
			draw = ImageDraw.Draw(im)

			font1 = ImageFont.truetype("data/fonts/m12.TTF", 32)
			font2 = ImageFont.truetype("data/fonts/V5PRD.TTF", 19)

			draw.text(((320/2) - (110/2), 40),"%s %02d" % (bank, screenNumber),(255,255,255),font=font1)
			draw.text(((320/2) - (110/2), 100),"F %02d" % frameNumber,(255,255,255),font=font1)
			draw.text((8, 150), "Temple of the Unticketed", (255, 255, 255), font=font2)

			im.save("data/test/bmp/%02d-%s%02d.bmp" % (frameNumber, bank, screenNumber))
			convert_cmds.append("convert bmp/%02d-%s%02d.bmp rgb/%02d-%s%02d.rgb" % (frameNumber, bank, screenNumber, frameNumber, bank, screenNumber))

with open("data/test/convert.sh", "w") as cfile:
	for cmd in convert_cmds:
		cfile.write(cmd + "\n")