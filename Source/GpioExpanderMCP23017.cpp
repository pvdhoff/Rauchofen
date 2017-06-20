/*
 * GpioExpanderMCP23017.cpp
 *
 *  Created on: 26.11.2013
 *      Author: pvh
 */

#include "GpioExpanderMCP23017.h"
#include <stdio.h>

GpioExpanderMCP23017::GpioExpanderMCP23017(int BusNr, unsigned int BusAddress)
: I2C_Access(BusNr,BusAddress)
, m_iBusNr(BusNr)
, m_uiBusAddress(BusAddress)
{

}

GpioExpanderMCP23017::~GpioExpanderMCP23017()
{
}

void GpioExpanderMCP23017::setDirectionIn(int Pin)
{
  if (isValidPin(Pin) )
  {
    int Reg = 0;
    if ( isPortA( Pin) )
    {
      Reg = getReg(DIRECTION_A);
      Reg = Reg | (0x1 << Pin);
      setReg(DIRECTION_A,Reg);
    }
    else
    {
      Reg = getReg(DIRECTION_B);
      Reg = Reg | (0x1 << (Pin - (MAXGPIOPIN / 2) ));
      setReg(DIRECTION_B,Reg);
    }
  }
}

bool GpioExpanderMCP23017::getDirectionIn(int Pin)
{
  if (isValidPin(Pin) )
  {
    int Reg = 0;
    int Mask = 0;
    if ( isPortA( Pin) )
    {
      Reg = getReg(DIRECTION_A);
      Mask = (0x1 << Pin);
    }
    else
    {
      Reg = getReg(DIRECTION_B);
      Mask = (0x1 << (Pin - (MAXGPIOPIN / 2) ));
    }
    if ( ( Reg & Mask ) == Mask)
      return true;
    else
      return false;
  }
  return false;
}

void GpioExpanderMCP23017::setDirectionOut(int Pin)
{
  if (isValidPin(Pin) )
  {
    int Reg = 0;
    if ( isPortA( Pin) )
    {
      Reg = getReg(DIRECTION_A);
      Reg = Reg & ~(0x1 << Pin);
      setReg(DIRECTION_A,Reg);
    }
    else
    {
      Reg = getReg(DIRECTION_B);
      Reg = Reg & ~(0x1 << (Pin - (MAXGPIOPIN / 2) ));
      setReg(DIRECTION_B,Reg);
    }
  }
}

bool GpioExpanderMCP23017::getDirectionOut(int Pin)
{
  if (isValidPin(Pin) )
  {
    int Reg = 0;
    int Mask = 0;
    if ( isPortA( Pin) )
    {
      Reg = getReg(DIRECTION_A);
      Mask = (0x1 << Pin);
    }
    else
    {
      Reg = getReg(DIRECTION_B);
      Mask = (0x1 << (Pin - (MAXGPIOPIN / 2) ));
    }
    if ( ( Reg & Mask ) == 0)
      return true;
    else
      return false;
  }
  return false;
}
int  GpioExpanderMCP23017::getValue(int Pin)
{
  if (isValidPin(Pin) )
  {
    //printf("Valid\n");
    int Reg = 0;
    int Mask = 0;
    if ( isPortA( Pin) )
    {
      if (getDirectionIn(Pin) == true )
      {
        //printf("In\n");
        Reg = getReg(GPIO_A);
      }
      else
      {
        //printf("Out\n");
        Reg = getReg(OLAT_A);
      }

      Mask = (0x1 << Pin);
    }
    else
    {
      if (getDirectionIn(Pin) == true )
        Reg = getReg(GPIO_B);
      else
       Reg = getReg(OLAT_B);

      Mask = (0x1 << (Pin - (MAXGPIOPIN / 2) ));
    }
    //printf("Mask=%d Reg=%d\n",Mask,Reg);
    if ( ( Reg & Mask ) == Mask)
      return 1;
    else
      return 0;
  }
  return 0;

}
void GpioExpanderMCP23017::setValue(int Pin, int val)
{
  if (isValidPin(Pin) && ( getDirectionOut(Pin) == true) )
  {
    int Reg = 0;
    if ( isPortA( Pin) )
    {
      Reg = getReg(OLAT_A);
      if (val == 0)
        Reg = Reg & ~(0x1 << Pin);
      else
        Reg = Reg | (0x1 << Pin);

      setReg(OLAT_A,Reg);
    }
    else
    {
      Reg = getReg(OLAT_B);
      if (val == 0)
        Reg = Reg & ~(0x1 << (Pin - (MAXGPIOPIN / 2) ));
      else
        Reg = Reg | (0x1 << (Pin - (MAXGPIOPIN / 2) ));

      setReg(OLAT_B,Reg);
    }
  }
}
void GpioExpanderMCP23017::setValuePattern(int Pattern)
{
  if (Pattern & GPIO_A_MASK)
    setReg(OLAT_A,Pattern & GPIO_A_MASK);

  if (Pattern & GPIO_B_MASK)
    setReg(OLAT_B,((Pattern & GPIO_A_MASK) >> GPIO_B_SHIFT));
}
int GpioExpanderMCP23017::getReg(int Addr)
{
  char reg[1];
  reg[0] = 0;
  readReg(reg, Addr);
  //printf("Reg=%d\n", (int)reg[0]);

  return (int)reg[0];
}

void GpioExpanderMCP23017::setReg(int Addr, int Value)
{
  char reg[1];
  reg[0] = (char)(Value & 0xFF);

  writeReg(reg, Addr);
}

bool GpioExpanderMCP23017::isPortA(int Pin)
{
  if (Pin < MAXGPIOPIN / 2)
    return true;
  else
    return false;
}

bool GpioExpanderMCP23017::isValidPin(int Pin)
{
  if (Pin < MAXGPIOPIN)
      return true;
    else
      return false;
}
