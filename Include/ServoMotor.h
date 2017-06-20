/*
 * ServoMotor.h
 *
 *  Created on: 03.11.2013
 *      Author: pvh
 */

#ifndef SERVOMOTOR_H_
#define SERVOMOTOR_H_

#define __DEFAULTMAX 500
#define __DEFAULTMIN 130

#include "IControllProcess.h"

class ServoController;

class ServoMotor : public IControllProcess
{
public:
  ServoMotor(ServoController* Controller, unsigned int Channel,unsigned int PulsMax,unsigned int PulsMin);
  ServoMotor(ServoController* Controller, unsigned int Channel);
  virtual ~ServoMotor();
  int SetServo(double Percent);
  int SetServo(unsigned int Puls);
  unsigned int GetMotorPulsMax();
  unsigned int GetMotorPulsMin();
  void SetMotorPulsMax(unsigned int Max);
  void SetMotorPulsMin(unsigned int Min);
  void ReverseAddjustValue(bool revert);
  int AdjustProcess(double value);
private:
  ServoController* m_Controller;
  unsigned int m_uiChannel;
  unsigned int m_uiPulsMax;
  unsigned int m_uiPulsMin;
  unsigned int m_uiPuls;
  bool         m_bRevert;
};

#endif /* SERVOMOTOR_H_ */
