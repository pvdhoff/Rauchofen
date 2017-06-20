/*
 * MainThread.h
 *
 *  Created on: 30.10.2014
 *      Author: pvh
 */

#ifndef MAINTHREAD_H_
#define MAINTHREAD_H_

#include "BaseThread.h"

class ControllData;
struct Message;

class MainThread: public BaseThread {
public:
	MainThread(ControllData* Data);
	virtual ~MainThread();
	virtual void run(void);
private:
	ControllData* m_pData;

	void ExcuteCommand(char* msg, int fd);
	void ParseCommand(struct Message* Msg, int fd);
	void SendAnswer(struct Message* Msg, int fd);
	void dataerror(const char *msg);
};

#endif /* MAINTHREAD_H_ */
