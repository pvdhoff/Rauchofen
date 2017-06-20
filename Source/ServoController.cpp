/*
 * ServoController.cpp
 *
 *  Created on: 01.11.2013
 *      Author: pvh
 */
#include <stdio.h>
#include <unistd.h>

#include <math.h>

#include "ServoController.h"
#include "ServoMotor.h"


ServoController::ServoController(int BusNr, unsigned int BusAddress)
: I2C_Access(BusNr,BusAddress)
, m_iBusNr(BusNr)
, m_uiBusAddress(BusAddress)
{
	int error = 0;
	error += ResetController();

	if(error == 0)
	  printf("Servo Contoller initializied\n");
	else
	  printf("Could not initialize Servo Controller\n");

	for(int i = 0; i < __NUMCHANNEL; i++)
	{
	  Motors.push_back(new ServoMotor(this,i));
	}

}

ServoController::~ServoController()
{
  char oldmode[1];
  if (readReg(oldmode, __MODE1) != 0)
  {
    printf("Connot read register %d\n",__MODE1);
  }
  char buff[1];
  buff[0] = (oldmode[0] & 0x7F) | 0x10;// sleep
  if (writeReg(buff, __MODE1) != 0)
  {
    printf("Connot write register %d\n",__MODE1);
  }

  for(int i = 0; i < __NUMCHANNEL; i++)
  {
    delete Motors[i];
  }

}
int ServoController::SetPWMFreqHz(double Freq){
	int Ret = 0;
	double prescaleval = __PRESCALEVAL;
	prescaleval /= 4096.0; // 12-bit
	prescaleval /= Freq;
	prescaleval -= 1.0;
	printf("Setting PWM frequency to %g Hz\n",Freq);
	printf("Estimated pre-scale: %g\n",prescaleval);
	unsigned int prescale = floor(prescaleval + 0.5);
	printf("Final pre-scale: %d\n", prescale);
	char oldmode[1];
	if (readReg(oldmode, __MODE1) != 0)
	{
		printf("Connot read register %d\n",__MODE1);
		return -1;
	}
	char buff[1];
	buff[0] = (oldmode[0] & 0x7F) | 0x10;// sleep
	if (writeReg(buff, __MODE1) != 0)
	{
		printf("Connot write register %d\n",__MODE1);
		return -1;
	}
	buff[0] = (char)( prescale & 0xFF);
	if (writeReg(buff, __PRESCALE) != 0)
	{
		printf("Connot write register %d\n",__PRESCALE);
		return -1;
	}
	if (writeReg(oldmode, __MODE1) != 0)
	{
		printf("Connot write register %d\n",__MODE1);
		return -1;
	}

	usleep(5000);
	oldmode[0] = (oldmode[0] | 0x80);
	if (writeReg(oldmode, __MODE1) != 0)
	{
		printf("Connot write register %d\n",__MODE1);
		return -1;
	}
	return Ret;
}
int ServoController::SetPWN(unsigned int channel, unsigned int Puls)
{
	//printf("Setting Channel %d to %d\n",channel,Puls);
	char buff[1];
	buff[0] = 0x00 & 0xFF;
	if (writeReg(buff, __LED0_ON_L+4*channel) != 0)
	{
		printf("Connot write register %d\n",__LED0_ON_L+4*channel);
		return -1;
	}
	buff[0] = 0x00 >> 8;
	if (writeReg(buff, __LED0_ON_H+4*channel) != 0)
	{
		printf("Connot write register %d\n",__LED0_ON_H+4*channel);
		return -1;
	}
	buff[0] = Puls & 0xFF;
	if (writeReg(buff, __LED0_OFF_L+4*channel) != 0)
	{
		printf("Connot write register %d\n",__LED0_OFF_L+4*channel);
		return -1;
	}
	buff[0] = Puls >> 8;
	if (writeReg(buff, __LED0_OFF_H+4*channel) != 0)
	{
		printf("Connot write register %d\n",__LED0_OFF_H+4*channel);
		return -1;
	}

	return 0;
}
ServoMotor* ServoController::GetServo(unsigned int Channel)
{
  if (Channel < 0) return NULL;
  if (Channel >= __NUMCHANNEL) return NULL;

  return Motors[Channel];

}

int ServoController::ResetController()
{
  int Ret = 0;
	printf("Resetting Device\n");
	char data[1];
	data[0] = 0x0;
  if ( writeReg(data, __MODE1) != 0)
  {
    Ret += -1;
  }
  if (Ret == 0)
    printf("Reset Done\n");

  return Ret;
}
