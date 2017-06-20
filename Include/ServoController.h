/*
 * ServoController.h
 *
 *  Created on: 01.11.2013
 *      Author: pvh
 */

#ifndef SERVOCONTROLLER_H_
#define SERVOCONTROLLER_H_

#include "I2CAccess.h"
#include "PCA9685.h"
#include <vector>

class ServoMotor;

class ServoController : public I2C_Access
{
public:
	ServoController(int BusNr, unsigned int BusAddress);
	virtual ~ServoController();
	int SetPWMFreqHz(double Freq);
	int SetPWN(unsigned int channel, unsigned int Puls);
	ServoMotor* GetServo(unsigned int Channel);
private:
	int  ResetController();
	int m_iBusNr;
	unsigned  int m_uiBusAddress;
	std::vector<ServoMotor*> Motors;
};

#endif /* SERVOCONTROLLER_H_ */
