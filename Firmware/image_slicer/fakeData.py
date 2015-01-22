
if __name__ == "__main__":
	with open("fake.bin", 'wb') as bin:
		b = bytearray()
		for _ in range(614400):
			b.append(0xaa)
			b.append(0xaa)
			b.append(0x55)
			b.append(0x55)

		bin.write(b)