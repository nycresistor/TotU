#==============================================================================================
# LED Matrix Animated Pattern Generator
# Copyright 2014 by Glen Akins.
# All rights reserved.
# 
# Set editor width to 96 and tab stop to 4.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#==============================================================================================

all: totu

totu: totu.o tft.o gpmc.o 
	g++ -o totu totu.o tft.o gpmc.o

totu.o: totu.c totu.h
	g++ -c totu.c

tft.o: tft.c tft.h
	g++ -c tft.c

gpmc.o: gpmc.c gpmc.h
	g++ -c gpmc.c

server.o: server.c
	g++ -c server.c

server: server.o gpmc.o tft.o
	g++ -o server tft.o gpmc.o server.o

clean:
	rm -rf totu totu.o tft.o gpmc.o server.o server
