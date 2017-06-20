/*
 * ADCMCP3008.h
 *
 *  Created on: 03.11.2013
 *      Author: pvh
 */

#ifndef ADCMCP3008_H_
#define ADCMCP3008_H_

#define __ADCCHANNELS 8

#include "SPIAccess.h"
#include <vector>

class ADC_Channel;
class IMeasureDevice;

class ADC_MCP3008 : SPI_Access
{
public:
  ADC_MCP3008(unsigned int Cs,unsigned char spiMode, unsigned int spiSpeed, unsigned char spibitsPerWord, unsigned int MeasureCycles);
  ~ADC_MCP3008();
  IMeasureDevice* GetADC_Channel(unsigned int Channel);
private:
  std::vector<ADC_Channel*> Channels;
};

#endif /* ADCMCP3008_H_ */
