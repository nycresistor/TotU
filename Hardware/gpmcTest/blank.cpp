#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <memory.h>
#include <iostream>
#include "blank.h"

#define OE_ADDR 0x134
#define GPIO_DATAOUT 0x13C
#define GPIO_DATAIN 0x138
#define GPIO0_ADDR 0x44E07000
#define GPIO1_ADDR 0x4804C000
#define GPIO2_ADDR 0x481AC000
#define GPIO3_ADDR 0x481AF000

#define DATA 0
#define CMD 1

void writeALot(uint16_t addr, uint16_t * data, uint16_t len);
void write8(uint8_t data);
void writeData8(uint8_t data);
void writeCommand8(uint8_t data);
void setDC(uint8_t dc);
void tft_setup(void);

int fd = 0;
int gfd = 0;

ulong * pinconf1;

int main (int argc, char *argv[])
{
    // open fpga memory device
    fd = open ("/dev/logibone_mem", O_RDWR | O_SYNC);

    gfd = open("/dev/mem", O_RDWR | O_SYNC);
    pinconf1 = (ulong*) mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, gfd, GPIO1_ADDR);
    
    pinconf1[OE_ADDR/4] &= (0xFFFFFFFF ^ (1 << 29));


    tft_setup();
	
/*
    uint16_t * data = (uint16_t*)malloc(256 * sizeof(uint16_t));
    for ( int i = 0; i < 256; i += 2) {
	data[i] = 0x00ff;
	data[i + 1] = 0;
    }    
    
    while (1) {
    	writeALot(0x0000, data, 256);
	usleep(10000);
    }
  
*/  
    close (fd);

    return 0;
}

void setDC(uint8_t dc)
{
    if (dc == CMD) {
	pinconf1[GPIO_DATAOUT/4] ^= (1 << 29);
    } else {
	pinconf1[GPIO_DATAOUT/4] |= (1 << 29);
    }
}

void tft_setup()
{
  writeCommand8(0xEF);
  writeData8(0x03);
  writeData8(0x80);
  writeData8(0x02);

  writeCommand8(0xCF);  
  writeData8(0x00); 
  writeData8(0XC1); 
  writeData8(0X30); 

  writeCommand8(0xED);  
  writeData8(0x64); 
  writeData8(0x03); 
  writeData8(0X12); 
  writeData8(0X81); 
 
  writeCommand8(0xE8);  
  writeData8(0x85); 
  writeData8(0x00); 
  writeData8(0x78); 

  writeCommand8(0xCB);  
  writeData8(0x39); 
  writeData8(0x2C); 
  writeData8(0x00); 
  writeData8(0x34); 
  writeData8(0x02); 
 
  writeCommand8(0xF7);  
  writeData8(0x20); 

  writeCommand8(0xEA);  
  writeData8(0x00); 
  writeData8(0x00); 
 
  writeCommand8(ILI9340_PWCTR1);    //Power control 
  writeData8(0x23);   //VRH[5:0] 
 
  writeCommand8(ILI9340_PWCTR2);    //Power control 
  writeData8(0x10);   //SAP[2:0];BT[3:0] 
 
  writeCommand8(ILI9340_VMCTR1);    //VCM control 
  writeData8(0x3e); //�Աȶȵ���
  writeData8(0x28); 
  
  writeCommand8(ILI9340_VMCTR2);    //VCM control2 
  writeData8(0x86);  //--
 
  writeCommand8(ILI9340_MADCTL);    // Memory Access Control 
  writeData8(ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);

  writeCommand8(ILI9340_PIXFMT);    
  writeData8(0x55); 
  
  writeCommand8(ILI9340_FRMCTR1);    
  writeData8(0x00);  
  writeData8(0x18); 
 
  writeCommand8(ILI9340_DFUNCTR);    // Display Function Control 
  writeData8(0x08); 
  writeData8(0x82);
  writeData8(0x27);  
 
  writeCommand8(0xF2);    // 3Gamma Function Disable 
  writeData8(0x00); 
 
  writeCommand8(ILI9340_GAMMASET);    //Gamma curve selected 
  writeData8(0x01); 
 
  writeCommand8(ILI9340_GMCTRP1);    //Set Gamma 
  writeData8(0x0F); 
  writeData8(0x31); 
  writeData8(0x2B); 
  writeData8(0x0C); 
  writeData8(0x0E); 
  writeData8(0x08); 
  writeData8(0x4E); 
  writeData8(0xF1); 
  writeData8(0x37); 
  writeData8(0x07); 
  writeData8(0x10); 
  writeData8(0x03); 
  writeData8(0x0E); 
  writeData8(0x09); 
  writeData8(0x00); 
  
  writeCommand8(ILI9340_GMCTRN1);    //Set Gamma 
  writeData8(0x00); 
  writeData8(0x0E); 
  writeData8(0x14); 
  writeData8(0x03); 
  writeData8(0x11); 
  writeData8(0x07); 
  writeData8(0x31); 
  writeData8(0xC1); 
  writeData8(0x48); 
  writeData8(0x08); 
  writeData8(0x0F); 
  writeData8(0x0C); 
  writeData8(0x31); 
  writeData8(0x36); 
  writeData8(0x0F); 

  writeCommand8(ILI9340_SLPOUT);    //Exit Sleep 
  usleep(120000); 		
  writeCommand8(ILI9340_DISPON);    //Display on 
}

void writeData8(uint8_t data)
{
    setDC(DATA);
    write8(data);	
}

void writeCommand8(uint8_t cmd)
{
    setDC(CMD);
    write8(cmd);
}

void write8(uint8_t data)
{
   uint8_t out; 
   for (int i = 7; i >= 0; i--) {	
    	out = data && (1 << i);
	pwrite(fd, &out, 1, 0);
    }
}

void writeALot(uint16_t addr, uint16_t * data, uint16_t len)
{
    pwrite (fd, data, len, addr);
}
