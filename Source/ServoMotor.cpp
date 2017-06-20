/*
 * ServoMotor.cpp
 *
 *  Created on: 03.11.2013
 *      Author: pvh
 */
#include <math.h>
#include "ServoMotor.h"
#include "ServoController.h"

ServoMotor::ServoMotor(ServoController* Controller, unsigned int Channel,unsigned int PulsMax,unsigned int PulsMin)
: m_Controller(Controller)
, m_uiChannel(Channel)
, m_uiPulsMax(PulsMax)
, m_uiPulsMin(PulsMin)
, m_uiPuls(0)
{
  m_Controller->SetPWN(m_uiChannel,0);

}

ServoMotor::ServoMotor(ServoController* Controller, unsigned int Channel)
: m_Controller(Controller)
, m_uiChannel(Channel)
, m_uiPulsMax(__DEFAULTMAX)
, m_uiPulsMin(__DEFAULTMIN)
, m_uiPuls(0)
, m_bRevert(false)
{
  m_Controller->SetPWN(m_uiChannel,0);

}

ServoMotor::~ServoMotor()
{

}

int ServoMotor::SetServo(double Percent)
{
  if (Percent > 100.0) Percent = 100.0;
  if (Percent < 0.0) Percent = 0.0;
  double OnePercent = ((double)(m_uiPulsMax - m_uiPulsMin)) / 100.0;

  unsigned int PulsLenth = floor((double)m_uiPulsMin + (Percent * OnePercent));
  if (Percent == 0)
    PulsLenth = 0;

  return SetServo(PulsLenth);
}

int ServoMotor::SetServo(unsigned int Puls)
{
  if (Puls == 0)
    m_uiPuls = Puls;
  else if (Puls > m_uiPulsMax)
    Puls = m_uiPulsMax;
  else if (Puls < m_uiPulsMin)
    Puls = m_uiPulsMin;

  m_uiPuls = Puls;

  return m_Controller->SetPWN(m_uiChannel,Puls);

}

unsigned int ServoMotor::GetMotorPulsMax(){return m_uiPulsMax;}
unsigned int ServoMotor::GetMotorPulsMin(){return m_uiPulsMin;}
void ServoMotor::SetMotorPulsMax(unsigned int Max){m_uiPulsMax = Max;}
void ServoMotor::SetMotorPulsMin(unsigned int Min){m_uiPulsMin = Min;}

void ServoMotor::ReverseAddjustValue(bool revert)
{
  m_bRevert = revert;
}
int ServoMotor::AdjustProcess(double value)
{
  unsigned int delta = m_uiPulsMax - m_uiPulsMin;
  double act_percent = (((double)(m_uiPuls-m_uiPulsMin)) / ((double)delta)) * 100.0;

  double setval = 0.0;
  if (m_bRevert == true)
  {
    setval = act_percent - value;
  }
  else
  {
    setval = act_percent + value;
  }

  if (setval > 100.0 || setval < 0.0)
  {
    SetServo(setval);
    return IControllProcess::OUT_OF_BOUND;
  }


  if (SetServo(setval) == 0)
  {
    return IControllProcess::OK;
  }
  else
  {
    return IControllProcess::OUT_OF_BOUND;
  }
}


