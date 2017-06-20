/*
 * ADCChannel.h
 *
 *  Created on: 03.11.2013
 *      Author: pvh
 */

#ifndef ADCCHANNEL_H_
#define ADCCHANNEL_H_

#include "IMeasureDevice.h"


class SPI_Access;

class ADC_Channel : public IMeasureDevice
{
public:
  ADC_Channel(SPI_Access* SPI_Access, unsigned int Channel, unsigned int MeasureCycles);
  virtual ~ADC_Channel();
  int Measure(double &dRetVal);
private:
  SPI_Access* m_SPI_Access;
  unsigned int m_uiChannel;
  unsigned int m_uiCycles;
};

#endif /* ADCCHANNEL_H_ */
