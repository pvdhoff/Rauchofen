/*
 * IMeasureDevice.h
 *
 *  Created on: 03.11.2013
 *      Author: pvh
 */

#ifndef IMEASUREDEVICE_H_
#define IMEASUREDEVICE_H_

class IMeasureDevice
{
public:
  enum returnval
  {
    VALID,
    NOT_VALID = -1
  };
  virtual int Measure(double &dRetVal) = 0;
};

#endif /* IMEASUREDEVICE_H_ */
