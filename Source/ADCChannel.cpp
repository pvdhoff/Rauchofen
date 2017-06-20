/*
 * ADCChannel.cpp
 *
 *  Created on: 03.11.2013
 *      Author: pvh
 */

#include "ADCChannel.h"
#include "ADCMCP3008.h"
#include <stdio.h>

ADC_Channel::ADC_Channel(SPI_Access* SPI_Access, unsigned int Channel, unsigned int MeasureCycles)
: m_SPI_Access(SPI_Access)
, m_uiChannel(Channel)
, m_uiCycles(MeasureCycles)
{

}

ADC_Channel::~ADC_Channel()
{

}

int ADC_Channel::Measure(double &dRetVal)
{
  unsigned int ReadVal;
  char Data[3];
  unsigned int sumval = 0;

  for(unsigned int i = 0; i < m_uiCycles + 1; i++)
  {
    Data[0] = 1;  //  first byte transmitted -> start bit
    Data[1] = 0b10000000 |( ((m_uiChannel & (__ADCCHANNELS -1)) << 4)); // second byte transmitted -> (SGL/DIF = 1, D2=D1=D0=0)
    Data[2] = 0; // third byte transmitted....don't care

    if(m_SPI_Access->SpiWriteRead(Data, sizeof(Data)) != 0 )
    {
      printf("SPI Read Write Failed \n");
      return IMeasureDevice::NOT_VALID;
    }

    if(0 == i)
    	continue;
    ReadVal = 0;
    ReadVal = (Data[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
    ReadVal |=  (Data[2] & 0xFF);
    sumval += ReadVal;
  }

  dRetVal = (double) sumval / (double) m_uiCycles;

  //Test PID
  //dRetVal = dRetVal / 10;

  return IMeasureDevice::VALID;
}

