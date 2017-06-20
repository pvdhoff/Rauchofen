/*
 * PCA9685.h
 *
 *  Created on: 02.11.2013
 *      Author: pvh
 */

#ifndef PCA9685_H_
#define PCA9685_H_

#define __SUBADR1      0x02
#define __SUBADR2      0x03
#define __SUBADR3      0x04
#define __MODE1        0x00
#define __PRESCALE     0xFE
#define __LED0_ON_L    0x06
#define __LED0_ON_H    0x07
#define __LED0_OFF_L   0x08
#define __LED0_OFF_H   0x09
#define __ALLLED_ON_L  0xFA
#define __ALLLED_ON_H  0xFB
#define __ALLLED_OFF_L 0xFC
#define __ALLLED_OFF_H 0xFD
#define __PRESCALEVAL  25000000.0 // 25MHz
#define __NUMCHANNEL   16




#endif /* PCA9685_H_ */
