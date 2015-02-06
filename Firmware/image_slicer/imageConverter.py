from PIL import Image
import sys, os, subprocess

openFolder = sys.argv[1]
files = os.listdir(openFolder)

if __name__ == "__main__":
	for path in files:
		print(path)
		filename = os.path.splitext(path)[0]
		image = Image.open(os.path.join(openFolder, path)).convert("RGB")

		with open(os.path.join("rgb", "%s.rgb" % filename), 'wb') as bin:
			pixels = image.load()
			for y in range(image.size[1]):
				for x in range(image.size[0]):
					bin.write(bytes([pixels[x, y][0]]))
					bin.write(bytes([pixels[x, y][1]]))
					bin.write(bytes([pixels[x, y][2]]))

	subprocess.call(["./slicer", "rgb"])