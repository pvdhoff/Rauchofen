/*
 * SPIAccess.cpp
 *
 *  Created on: 03.11.2013
 *      Author: pvh
 */

#include "SPIAccess.h"

#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <string.h>

SPI_Access::SPI_Access(unsigned int Cs,unsigned char spiMode, unsigned int spiSpeed, unsigned char spibitsPerWord)
: m_fd(-1)
, m_uiCs(Cs)
, m_ucSpiMode(spiMode)
, m_uiSpiSpeed(spiSpeed)
, m_ucBitsPerWord(spibitsPerWord)
{
  snprintf(m_filename, 19, "/dev/spidev0.%d", m_uiCs);

}
SPI_Access::~SPI_Access()
{
  closeDriver();
}

int SPI_Access::SpiWriteRead(char* Data, int length)
{
  if(DriverIsOpen() == true)
  {
    struct spi_ioc_transfer spi[length];
    int i = 0;
    int retVal = -1;

    // one spi transfer for each byte
    for (i = 0 ; i < length ; i++){
      memset(&spi[i],0,sizeof(struct spi_ioc_transfer));
      spi[i].tx_buf        = (unsigned long)(Data + i); // transmit from "data"
      spi[i].rx_buf        = (unsigned long)(Data + i) ; // receive into "data"
      spi[i].len           = sizeof(char) ;
      spi[i].delay_usecs   = 0 ;
      spi[i].speed_hz      = m_uiSpiSpeed ;
      spi[i].bits_per_word = m_ucBitsPerWord ;
      spi[i].cs_change = 0;
    }
    retVal = ioctl (m_fd, SPI_IOC_MESSAGE(length), &spi) ;

    if(retVal < 0){
      printf("Problem transmitting spi data..ioctl\n");
      return -1;
    }

    closeDriver();
    return 0;
  }
  else
  {
    closeDriver();
    return -1;
  }

}

int SPI_Access::openDriver()
{
  int Ret = 0;
  m_fd = open(m_filename, O_RDWR);
  if(m_fd < 0){
      printf("could not open SPI device\n");
      return -1;
  }

  Ret = ioctl (m_fd, SPI_IOC_WR_MODE, &m_ucSpiMode);
  //Ret = ioctl (m_fd, SPI_IOC_WR_MODE32, &m_ucSpiMode);
  if(Ret < 0){
      printf("Could not set SPIMode (WR)...ioctl fail\n");
      return -1;
  }

  Ret = ioctl (m_fd, SPI_IOC_WR_MODE, &m_ucSpiMode);
  //Ret = ioctl (m_fd, SPI_IOC_WR_MODE32, &m_ucSpiMode);
  if(Ret < 0) {
    printf("Could not set SPIMode (RD)...ioctl fail\n");
    return -1;
  }

  Ret = ioctl (m_fd, SPI_IOC_WR_BITS_PER_WORD, &m_ucBitsPerWord);
  if(Ret < 0) {
    printf("Could not set SPI bitsPerWord (WR)...ioctl fail\n");
    return -1;
  }

  Ret = ioctl (m_fd, SPI_IOC_RD_BITS_PER_WORD, &m_ucBitsPerWord);
  if(Ret < 0) {
    printf("Could not set SPI bitsPerWord(RD)...ioctl fail\n");
    return -1;
  }

  Ret = ioctl (m_fd, SPI_IOC_WR_MAX_SPEED_HZ, &m_uiSpiSpeed);
  if(Ret < 0) {
    printf("Could not set SPI speed (WR)...ioctl fail\n");
    return -1;
  }

  Ret = ioctl (m_fd, SPI_IOC_RD_MAX_SPEED_HZ, &m_uiSpiSpeed);
  if(Ret < 0) {
    printf("Could not set SPI speed (RD)...ioctl fail\n");
    return -1;
  }
  return Ret;
}

int SPI_Access::closeDriver()
{
  int statusVal = -1;
  int fd = m_fd;
  m_fd = -1;
  if(fd >= 0)
  {
    statusVal = close(fd);
    if(statusVal < 0) {
      printf("Could not close SPI device\n");
      return -1;
    }
    return statusVal;
  }
  else
  {
    return 0;
  }
}

bool SPI_Access::DriverIsOpen()
{
  if(m_fd >= 0)
  {
    return true;
  }
  else
  {
  if(openDriver() == 0)
    return true;
  else
    return false;
  }
}
