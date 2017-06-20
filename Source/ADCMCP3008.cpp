/*
 * ADCMCP3008.cpp
 *
 *  Created on: 03.11.2013
 *      Author: pvh
 */

#include "ADCMCP3008.h"
#include "ADCChannel.h"

#include <unistd.h>
#include <stdint.h>

ADC_MCP3008::ADC_MCP3008(unsigned int Cs,unsigned char spiMode, unsigned int spiSpeed, unsigned char spibitsPerWord, unsigned int MeasureCycles)
: SPI_Access( Cs, spiMode, spiSpeed, spibitsPerWord)
{
  for(unsigned int i = 0; i < __ADCCHANNELS; i++)
  {
    Channels.push_back(new ADC_Channel(this,i,MeasureCycles));

  }
}

ADC_MCP3008::~ADC_MCP3008()
{

}
IMeasureDevice* ADC_MCP3008::GetADC_Channel(unsigned int Channel)
{
  if(Channel < 0){return NULL;}
  if(Channel >= __ADCCHANNELS){return NULL;}

  return Channels[Channel];
}
