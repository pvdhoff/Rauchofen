/*
 * I2CAccess.cpp
 *
 *  Created on: 02.11.2013
 *      Author: pvh
 */

#include "I2CAccess.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>



I2C_Access::I2C_Access(int BusNr, unsigned int BusAddress)
: m_fd(-1)
, m_iBusNr(BusNr)
, m_uiBusAddress(BusAddress)
{
	snprintf(m_filename, 19, "/dev/i2c-%d", BusNr);
	int error = 0;
	error += openDriver();
	error += closeDriver();

}

I2C_Access::~I2C_Access()
{
	closeDriver();
}

int I2C_Access::writeReg(char *data, int offset)
{
  int ret;
  if (DriverIsOpen() == true)
  {
	  struct i2c_rdwr_ioctl_data i2c_data;
	  struct i2c_msg msg[1];
	  unsigned char *buf = NULL;
	  buf = (unsigned char*)malloc(2); // offset + data

	  buf[0] = offset;
	  memcpy(&buf[1], data, 1);

	  /*
	  * write operation
	  */
	  i2c_data.msgs = msg;
	  i2c_data.nmsgs = 1;             // use one message structure

	  i2c_data.msgs[0].addr = m_uiBusAddress;
	  i2c_data.msgs[0].flags = 0;     // don't need flags
	  i2c_data.msgs[0].len = 2;
	  i2c_data.msgs[0].buf = (__u8 *)buf;

	  /*
	  * ioctl() processes read & write.
	  * Operation is determined by flags field of i2c_msg
	  */
	  if((ret = ioctl(m_fd, I2C_RDWR, &i2c_data)) < 0) {
		printf("write data fail\n");
		closeDriver();
		return ret;
	  }
	  //printf("Writing data to %d succsessful\n", offset);

	  free(buf);

	  closeDriver();
	  return 0;
  }
  else
  {
    closeDriver();
	  return -1;
  }
}

int I2C_Access::readReg(char *data, int offset)
{
  int ret;
  if (DriverIsOpen() == true)
  {
    struct i2c_rdwr_ioctl_data i2c_data;
    struct i2c_msg msg[2];
    char tmp[2];

    /*
    * Two operation is necessary for read operation.
    * First determine offset address
    * , and then receive data.
    */
    i2c_data.msgs = msg;
    i2c_data.nmsgs = 2;     // two i2c_msg

    tmp[0] = offset;
    i2c_data.msgs[0].addr = m_uiBusAddress;
    i2c_data.msgs[0].flags = 0;     // write
    i2c_data.msgs[0].len = 1;       // only one byte
    i2c_data.msgs[0].buf = (__u8 *)tmp;

    /*
    * Second, read data from the EEPROM
    */
    i2c_data.msgs[1].addr = m_uiBusAddress;
    i2c_data.msgs[1].flags = I2C_M_RD;      // read command
    i2c_data.msgs[1].len = 1;
    i2c_data.msgs[1].buf = (__u8 *)data;


    ret = ioctl(m_fd, I2C_RDWR, &i2c_data);

    if (ret < 0)
    {
      printf("read data fail\n");
      closeDriver();
      return ret;
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


int I2C_Access::openDriver()
{
	int Ret = 0;
	m_fd = open(m_filename, O_RDWR);
	if (m_fd < 0)
	{
	  printf("Cannot open Device %s\n",m_filename);
	  Ret -=1;
	}
	else
	{
	  //if (ioctl(m_fd, I2C_SLAVE, m_uiBusAddress) < 0)
	  //{
	  //  printf("IOCTRL(I2C_SLAVE) failed");
		//  Ret -=1;
	  //}
	}
	if (Ret == 0)
	{
	  //printf("Device %s succsessfuly opened\n",m_filename);
	}
	return Ret;
}

int I2C_Access::closeDriver()
{
	int Ret = 0;
	int fd = m_fd;
	m_fd = -1;
	if (fd >= 0)
	{
	  Ret += close(fd);
	}

	if (Ret != 0)
	{
		//printf("Cannot close Device %s\n",m_filename);
	}
	else
	{
	  //printf("Device %s closed\n",m_filename);
	}
	return Ret;
}

bool I2C_Access::DriverIsOpen()
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

