#ifndef _gpmc_h_
#define _gpmc_h_

#include <stdint.h>

#define DATA 0
#define CMD 1

#define BLOCK_SIZE 16384

void setup_gpmc(void);
// void writeALot(uint16_t addr, uint16_t * data, uint16_t len);
void write8(uint8_t data);
void writeData8(uint8_t data);
void writeCommand8(uint8_t data);
void gpmcWrite(uint16_t * data, uint32_t len);
void close_gpmc(void);
void writeByte(uint8_t data);
#endif
