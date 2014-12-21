#include "tft.h"
#include "stdio.h"

ulong * gpio1;
ulong * gpio2;
int gfd;

#define set_gpio(gpio, num) gpio[GPIO_DATAOUT/4] |= (1 << num)
#define clear_gpio(gpio, num) gpio[GPIO_DATAOUT/4] &= ~(1 << num)

#define RESET   31
#define DC      29
#define MUXS_0  24
#define MUXS_1  22

void begin_tft()
{
  // Configure TFT Data/Command line (GPIO1_29 P8.26)
  // TFT Reset line (GPIO1_31 P8.20)
  // and MUX_S0 (GPIO2_24 P8.28) and MUXS_1 (GPIO2_22 P8.27)
  printf("Configuring DC and reset lines\n");
  gfd = open("/dev/mem", O_RDWR | O_SYNC);
  gpio1 = (ulong*) mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, gfd, GPIO1_ADDR);
  gpio2 = (ulong*) mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, gfd, GPIO2_ADDR);
  
 // gpio1[OE_ADDR/4] &= (0xFFFFFFFF ^ ((1 << DC) | (1 << RESET)));
 // gpio2[OE_ADDR/4] &= (0xFFFFFFFF ^ ((1 << MUXS_0) | (1 << MUXS_1)));
}

void reset_tft()
{
  printf("Resetting TFT\n");
  // pinconf1[GPIO_DATAOUT/4] |= (1 << 31);
  set_gpio(gpio1, RESET);
  usleep(100000);
  // pinconf1[GPIO_DATAOUT/4] ^= (1 << 31);
  clear_gpio(gpio1, RESET);
  usleep(100000);
  // pinconf1[GPIO_DATAOUT/4] |= (1 << 31);
  set_gpio(gpio1, RESET);
  usleep(100000);
}

void activateBank(int bank)
{
	if (bank == 0) {
		set_gpio(gpio2, MUXS_0);
		clear_gpio(gpio2, MUXS_1);
	} else {
		clear_gpio(gpio2, MUXS_0);
		set_gpio(gpio2, MUXS_1);
	}
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
	// pinconf1[GPIO_DATAOUT/4] &= ~(1 << 29);
      clear_gpio(gpio1, DC);
    } else {
//	printf("DC going HIGH\n");
	// pinconf1[GPIO_DATAOUT/4] |= (1 << 29);
      set_gpio(gpio1, DC);
    }
}

void writeFramePregenerated(uint16_t * screen, uint32_t len)
{
	setDC(DATA);
	gpmcWritePregenerated(screen, len);

}
