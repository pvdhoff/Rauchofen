/*
 * ControllData.cpp
 *
 *  Created on: 30.10.2014
 *      Author: pvh
 */

#include "ControllData.h"
#include <stdio.h>
#include <string.h>

ControllData::ControllData() :
  m_iError(0)
{
	memset(m_dTemp,0,MAX_TEMP_SENSOR * sizeof(double));
	memset(m_dADC,0,MAX_ADC_CHANNEL * sizeof(double));
	memset(m_iServo,0,MAX_SERVO_CHANNEL * sizeof(int));
	pthread_mutex_init (&mutex, NULL);
}

ControllData::~ControllData() {
	// TODO Auto-generated destructor stub
}
double ControllData::GetTemperatur(unsigned int channel)
{
	double tmp = 0.0;
	if(channel >= MAX_TEMP_SENSOR)
		return tmp;

	pthread_mutex_lock (&mutex);
	tmp = m_dTemp[channel];
	pthread_mutex_unlock (&mutex);

	return tmp;
}
double ControllData::GetADC(unsigned int channel)
{
	double tmp = 0.0;
	if(channel >= MAX_ADC_CHANNEL)
		return tmp;

	pthread_mutex_lock (&mutex);
	tmp = m_dADC[channel];
	pthread_mutex_unlock (&mutex);

	return tmp;
}
int    ControllData::GetServo(unsigned int channel)
{
	int tmp = 0;
	if(channel >= MAX_SERVO_CHANNEL)
		return tmp;

	pthread_mutex_lock (&mutex);
	tmp = m_iServo[channel];
	pthread_mutex_unlock (&mutex);

	return tmp;
}
int    ControllData::GetError(void)
{
	int tmp = 0;

	pthread_mutex_lock (&mutex);
	tmp = m_iError;
	pthread_mutex_unlock (&mutex);

	return tmp;
}
void   ControllData::SetTemperatuer(unsigned int channel, double Temp)
{
	if(channel >= MAX_TEMP_SENSOR)
		return;

	pthread_mutex_lock (&mutex);
	m_dTemp[channel] = Temp;
	pthread_mutex_unlock (&mutex);
}
void   ControllData::SetADC(unsigned int channel, double value)
{
	if(channel >= MAX_ADC_CHANNEL)
		return;

	pthread_mutex_lock (&mutex);
	m_dADC[channel] = value;
	pthread_mutex_unlock (&mutex);
}
void   ControllData::SetServo(unsigned int channel, int value)
{
	if(channel >= MAX_SERVO_CHANNEL)
		return;

	pthread_mutex_lock (&mutex);
	m_iServo[channel] = value;
	pthread_mutex_unlock (&mutex);
}
void   ControllData::SetError(int Error)
{
	pthread_mutex_lock (&mutex);
	m_iError = Error;
	pthread_mutex_unlock (&mutex);
}

