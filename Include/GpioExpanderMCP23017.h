/*
 * GpioExpanderMCP23017.h
 *
 *  Created on: 26.11.2013
 *      Author: pvh
 */

#ifndef GPIOEXPANDERMCP23017_H_
#define GPIOEXPANDERMCP23017_H_

#define MAXGPIOPIN 16

#define DIRECTION_A 0x0
#define DIRECTION_B 0x1

#define OLAT_A 0x14
#define OLAT_B 0x15

#define GPIO_A 0x12
#define GPIO_B 0x13

#define GPIO_A_MASK 0xFF
#define GPIO_B_MASK 0xFF00

#define GPIO_B_SHIFT 8

#include "I2CAccess.h"

class GpioExpanderMCP23017 : public I2C_Access
{
public:
  GpioExpanderMCP23017(int BusNr, unsigned int BusAddress);
  virtual ~GpioExpanderMCP23017();

  void setDirectionIn(int Pin);
  bool getDirectionIn(int Pin);

  void setDirectionOut(int Pin);
  bool getDirectionOut(int Pin);

  int  getValue(int Pin);
  void setValue(int Pin, int Value);
  void setValuePattern(int Pattern);



private:
  int m_iBusNr;
  unsigned  int m_uiBusAddress;
  int  getReg(int Addr);
  void setReg(int Addr, int Value);
  bool isPortA(int Pin);
  bool isValidPin(int Pin);
};

#endif /* GPIOEXPANDERMCP23017_H_ */
