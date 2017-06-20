/*
 * ControllData.h
 *
 *  Created on: 30.10.2014
 *      Author: pvh
 */

#ifndef CONTROLLDATA_H_
#define CONTROLLDATA_H_

#include <pthread.h>
#define MAX_TEMP_SENSOR 10
#define MAX_ADC_CHANNEL 8
#define MAX_SERVO_CHANNEL 16

class ControllData {
public:
	ControllData();
	virtual ~ControllData();
	double GetTemperatur(unsigned int channel);
	double GetADC(unsigned int channel);
	int	   GetServo(unsigned int channel);
	int	   GetError(void);
	void   SetTemperatuer(unsigned int channel, double value);
	void   SetADC(unsigned int channel, double value);
	void   SetServo(unsigned int channel, int value);
	void   SetError(int Error);
private:
	double m_dTemp[MAX_TEMP_SENSOR];
	double m_dADC[MAX_ADC_CHANNEL];
	int    m_iServo[MAX_SERVO_CHANNEL];
	int    m_iError;
	pthread_mutex_t mutex;
};

#endif /* CONTROLLDATA_H_ */
