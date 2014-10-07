#include "tft.h"
#include "stdio.h"

ulong * pinconf1;
int gfd;


void reset_tft()
{
  printf("Resetting TFT\n");
  pinconf1[GPIO_DATAOUT/4] |= (1 << 31);
  usleep(100000);
  pinconf1[GPIO_DATAOUT/4] ^= (1 << 31);
  usleep(100000);
  pinconf1[GPIO_DATAOUT/4] |= (1 << 31);
  usleep(100000);
}

void begin_tft()
{
  printf("Configuring DC and reset lines\n"); // GPIO 1_29 P8.26 and 1_31 P8.20
  gfd = open("/dev/mem", O_RDWR | O_SYNC);
  pinconf1 = (ulong*) mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, gfd, GPIO1_ADDR);
  pinconf1[OE_ADDR/4] &= (0xFFFFFFFF ^ ((1 << 29) | (1 << 31)));

}

void setup_tft()
{
 
  begin_tft();  
  reset_tft();
 
  printf("Sending display setup commands\n");
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
  usleep(100000); 		
  writeCommand8(ILI9340_DISPON);    //Display on 
}

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{

  printf("Setting adder window to %d %d %d %d\n", x0, y0, x1, y1);

  writeCommand8(ILI9340_CASET);
  writeData8(x0 >> 8);
  writeData8(x0 & 0xFF);     // XSTART 
  writeData8(x1 >> 8);
  writeData8(x1 & 0xFF);     // XEND

  writeCommand8(ILI9340_PASET); // Row addr set
  writeData8(y0>>8);
  writeData8(y0);     // YSTART
  writeData8(y1>>8);
  writeData8(y1);     // YEND

  writeCommand8(ILI9340_RAMWR); // write to RAM

}

void setRotation(uint8_t m)
{
  printf("Setting rotation %d\n", m);
  writeCommand8(ILI9340_MADCTL);
  switch (m) {
   case 0:
     writeData8(ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);
     //_width  = ILI9340_TFTWIDTH;
     //_height = ILI9340_TFTHEIGHT;
     break;
   case 1:
     writeData8(ILI9340_MADCTL_MV | ILI9340_MADCTL_BGR);
     //_width  = ILI9340_TFTHEIGHT;
     //_height = ILI9340_TFTWIDTH;
     break;
  case 2:
    writeData8(ILI9340_MADCTL_MY | ILI9340_MADCTL_BGR);
     //_width  = ILI9340_TFTWIDTH;
     //_height = ILI9340_TFTHEIGHT;
    break;
   case 3:
     writeData8(ILI9340_MADCTL_MV | ILI9340_MADCTL_MY | ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);
     //_width  = ILI9340_TFTHEIGHT;
     //_height = ILI9340_TFTWIDTH;
     break;
  }
}

void writeData8(uint8_t data)
{
    setDC(DATA);
    write8(data);	
}

void writeCommand8(uint8_t cmd)
{
    printf("Writing command %02x\n", cmd);
    setDC(CMD);
    write8(cmd);
}

void setDC(uint8_t dc)
{
    if (dc == CMD) {
//	printf("DC going LOW\n");
	pinconf1[GPIO_DATAOUT/4] &= ~(1 << 29);
    } else {
//	printf("DC going HIGH\n");
	pinconf1[GPIO_DATAOUT/4] |= (1 << 29);
    }
}

void writeFrame(uint16_t * screen)
{
    setDC(DATA);
    gpmcWrite(screen, WIDTH * HEIGHT); 

}

void writeFramePregenerated(uint16_t * screen)
{
	setDC(DATA);
	gpmcWritePregenerated(screen, WIDTH * HEIGHT);

}
