/*
 * MainThread.cpp
 *
 *  Created on: 30.10.2014
 *      Author: pvh
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio_ext.h>
#include <netinet/in.h>
#include <string>
#include <signal.h>
#include <unistd.h>

#include "MainThread.h"
#include "ControllData.h"
#include "ControllProtokoll.h"

MainThread::MainThread(ControllData* Data) :
    m_pData(Data)
{

}

MainThread::~MainThread() {
	// TODO Auto-generated destructor stub
}

void MainThread::run()
{
	printf("Main Thread Loop started\n");
	int sockfd, newsockfd;
	socklen_t clilen;
	struct sockaddr_in  cli_addr, serv_addr;
	char buf[256];
	signal(SIGPIPE, SIG_IGN);
	int portno = 5025;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		dataerror("creating socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
	{
		close(sockfd);
		dataerror("binding socket");
		m_pData->SetError(-1);
		return;
	}

   if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL)) < 0)
   {
	 close(sockfd);
	 dataerror("NONBLOCKING");
	 m_pData->SetError(-1);
	 return ;
   }

   listen(sockfd,5);

   clilen = sizeof(cli_addr);
   printf("Waiting for messages\n");

   while (1)
   {
	 bool usesocket = false;
	 newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);

	 if (newsockfd < 0)
	 {
	   usesocket = false;
	   printf("No connection has been established\n");
	 }
	 else
	 {
	   usesocket = true;
	   //printf("A connection has been established\n");
	 }

	 if (usesocket == true)
	 {
		 int nin;
		 memset(buf,0,sizeof(buf));
		 nin = read(newsockfd,buf,sizeof(buf));
		 if(0 == nin)
		 {
			 dataerror("Exit Threads");
			 m_pData->SetError(-1);
			 return;
		 }
		 else if(nin == sizeof(struct Message))
		 {
			 //printf("Data rec: ");
			 //for (int i = 0; i < 12; i++ )
				 //printf("0x%02x ", buf[i]);

			 //printf("\n");

			 ExcuteCommand(buf, newsockfd);
		 }
	 }

	 if (usesocket == true)
	 {
	   close(newsockfd);
	   //printf("A connection has been closed\n");
	 }
   }
   close(sockfd);


}

void MainThread::dataerror(const char *msg)
{
    perror(msg);
}

void MainThread::ExcuteCommand(char* msg, int fd)
{
	struct Message sMsg;
	memset(&sMsg,0,sizeof(struct Message));
	memcpy(&sMsg,msg,sizeof(struct Message));
	ParseCommand(&sMsg,fd);
	return;
}
void MainThread::ParseCommand(struct Message* Msg, int fd)
{
	int channel = static_cast<int>(Msg->Channel);
	switch(Msg->Msg)
	{
	case GET_TEMP_VAL:
	{
		double Temp = m_pData->GetTemperatur(channel);
		char answer[256];
		memset(answer,0,256);
		struct Message* rMsg = (struct Message*)answer;
		rMsg->Msg = Msg->Msg;
		rMsg->Channel = Msg->Channel;
		memcpy(&(rMsg->Value),&Temp,sizeof(double));
		SendAnswer(rMsg, fd);
		break;
	}
	case SET_TEMP_VAL:
	{
		double* Value = (double*)&(Msg->Value);
		double Buffer = *Value;
		m_pData->SetTemperatuer(channel,Buffer);
		memset(&(Msg->Value),0,MAX_VALUE_SIZE);
		SendAnswer(Msg, fd);
		break;
	}
	case GET_ADC_VAL:
	{
		double ADC = m_pData->GetADC(channel);
		char answer[256];
		memset(answer,0,256);
		struct Message* rMsg = (struct Message*)answer;
		rMsg->Msg = Msg->Msg;
		rMsg->Channel = Msg->Channel;
		memcpy(&(rMsg->Value),&ADC,sizeof(double));
		SendAnswer(rMsg, fd);
		break;
	}
	case SET_ADC_VAL:
	{
		double* Value = (double*)&(Msg->Value);
		double Buffer = *Value;
		m_pData->SetADC(channel,Buffer);
		memset(&(Msg->Value),0,MAX_VALUE_SIZE);
		SendAnswer(Msg, fd);
		break;
	}
	case GET_SERVO_VAL:
	{
		int Value = m_pData->GetServo(channel);
		char answer[256];
		memset(answer,0,256);
		struct Message* rMsg = (struct Message*)answer;
		rMsg->Msg = Msg->Msg;
		rMsg->Channel = Msg->Channel;
		memcpy(&(rMsg->Value),&Value,sizeof(int));
		SendAnswer(rMsg, fd);
		break;
	}
	case SET_SERVO_VAL:
	{
		int* Value = (int*)&(Msg->Value);
		int Buffer = *Value;
		m_pData->SetServo(channel,Buffer);
		memset(&(Msg->Value),0,MAX_VALUE_SIZE);
		SendAnswer(Msg, fd);
		break;
	}
	default:
		break;
	}
	return;
}
void MainThread::SendAnswer(struct Message* Msg, int fd)
{
	int nout = write(fd,(char*)Msg,sizeof(struct Message));
	if(nout != sizeof(struct Message))
		dataerror("Error writing Answer\n");
	return;
}

