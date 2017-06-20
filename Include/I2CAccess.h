/*
 * I2CAccess.h
 *
 *  Created on: 02.11.2013
 *      Author: pvh
 */

#ifndef I2CACCESS_H_
#define I2CACCESS_H_

class I2C_Access {
public:
	I2C_Access(int BusNr, unsigned int BusAddress);
	virtual ~I2C_Access();
	int  writeReg(char *data, int offset);
	int  readReg(char *data, int offset);
private:
	int  openDriver();
	int  closeDriver();
	bool DriverIsOpen();
	int m_fd;
	int m_iBusNr;
	char m_filename[20];
	unsigned  int m_uiBusAddress;
};

#endif /* I2CACCESS_H_ */
