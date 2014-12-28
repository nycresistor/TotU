all: slicer

slicer: slicer.o
	g++ -o slicer slicer.o

slicer.o:
	g++ -c slicer.c

clean:
	rm -rf slicer slicer.o