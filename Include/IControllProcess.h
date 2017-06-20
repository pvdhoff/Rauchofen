/*
 * IControllProcess.h
 *
 *  Created on: 03.11.2013
 *      Author: pvh
 */

#ifndef ICONTROLLPROCESS_H_
#define ICONTROLLPROCESS_H_

class IControllProcess
{
public:
  enum returnval
  {
    OK,
    OUT_OF_BOUND = -1
  };
  virtual int AdjustProcess(double value) = 0;
  virtual void ReverseAddjustValue(bool revert) = 0;
};

#endif /* ICONTROLLPROCESS_H_ */
