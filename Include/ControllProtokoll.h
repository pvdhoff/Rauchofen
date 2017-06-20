/*
 * ControllProtokoll.h
 *
 *  Created on: 31.10.2014
 *      Author: pvh
 */

#ifndef CONTROLLPROTOKOLL_H_
#define CONTROLLPROTOKOLL_H_

#define MAX_VALUE_SIZE 10

#define GET_TEMP_VAL   1
#define SET_TEMP_VAL   2
#define GET_ADC_VAL    3
#define SET_ADC_VAL    4
#define GET_SERVO_VAL  5
#define SET_SERVO_VAL  6

struct Message
{
	unsigned char Msg;
	unsigned char Channel;
	unsigned char Value[10];
};



#endif /* CONTROLLPROTOKOLL_H_ */
