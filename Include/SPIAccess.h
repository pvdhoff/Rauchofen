/*
 * SPIAccess.h
 *
 *  Created on: 03.11.2013
 *      Author: pvh
 */

#ifndef SPIACCESS_H_
#define SPIACCESS_H_

#include <linux/spi/spidev.h>
//#include "spidev.h"

class SPI_Access {
public:
  SPI_Access(unsigned int Cs,unsigned char spiMode, unsigned int spiSpeed, unsigned char spibitsPerWord);
  ~SPI_Access();
  int SpiWriteRead(char* Data, int length);
private:
  int  openDriver();
  int  closeDriver();
  bool DriverIsOpen();
  int m_fd;
  unsigned int m_uiCs;
  unsigned char m_ucSpiMode;
  unsigned int m_uiSpiSpeed;
  unsigned char m_ucBitsPerWord;
  char m_filename[20];
};

#endif /* SPIACCESS_H_ */
