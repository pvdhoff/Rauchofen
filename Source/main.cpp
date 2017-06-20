/*
 * main.cpp
 *
 *  Created on: 30.10.2014
 *      Author: pvh
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "MainThread.h"
#include "W1DS.h"
#include "ControllData.h"
#include "GpioExpanderMCP23017.h"
#include "ServoController.h"
#include "ServoMotor.h"
#include "ADCMCP3008.h"
#include "IMeasureDevice.h"
#include "rrdarchive.h"

int main(int argc, char** argv)
{
	printf("Starting Räucherofen-Controll....\n");
	ControllData* pData = new ControllData();
	// GPIO
	GpioExpanderMCP23017* Gpio = new GpioExpanderMCP23017( 1, 0x20 );
	Gpio->setDirectionOut(0);
	Gpio->setValue(0,1);

	// Servomotoren
	int Servo1 = 0;
	int Motor  = 0;
	ServoController* Controller = new ServoController(1,0x40);
	Controller->SetPWMFreqHz(50.0);
	ServoMotor* Motor_0 = Controller->GetServo(0);
	Motor_0->SetMotorPulsMax(500);
	Motor_0->SetMotorPulsMin(260);
	Motor_0->SetServo(static_cast<double>(Servo1));
	pData->SetServo(0,Servo1);

	ServoMotor* Motor_3 = Controller->GetServo(3);
	Motor_3->SetMotorPulsMax(4090);
	Motor_3->SetMotorPulsMin(1800);
	Motor_3->SetServo(static_cast<double>(Motor));
	pData->SetServo(3,Motor);

	ADC_MCP3008* ADC = new ADC_MCP3008(0, SPI_MODE_0, 1000000, 8, 10);
	IMeasureDevice* Poti = ADC->GetADC_Channel(0);
	IMeasureDevice* Optic = ADC->GetADC_Channel(1);
	double MeasuredValue = 0.0;
	Poti->Measure(MeasuredValue);
	printf("Poti: %g\n",MeasuredValue);
	Optic->Measure(MeasuredValue);
	printf("Optic: %g\n",MeasuredValue);

	// Temperatur Sensor
	//W1_DS* pTempSensor = new W1_DS((char*)"10-0008028a815e");
	W1_DS* pTempSensor = new W1_DS((char*)"10-00080299c9f4");
	double Temp = 0.0;
	int ret = pTempSensor->ReadTemp(Temp);
	if(ret)
	{
		printf("Error reading Temperatur\n");
	}
	else
	{
		//printf("Initiale Temperatur: %g\n",Temp);
	}

	pData->SetTemperatuer(0,Temp);

	rrd_archive* pRra = new rrd_archive(std::string("Rauchofen.rrd"), 1, false);
	pRra->add_Ds(std::string("temp"), 10, rrd_archive::eDsGAUGE, -200.0, 200.0);
	pRra->add_Ds(std::string("rauch"), 10, rrd_archive::eDsGAUGE, 0.0, 65535.0);

	pRra->add_RraHours(rrd_archive::eRraAVERAGE, 0.5, 10, 6);
	pRra->add_RraHours(rrd_archive::eRraMIN, 0.5, 10, 6);
	pRra->add_RraHours(rrd_archive::eRraMAX, 0.5, 10, 6);

	pRra->add_RraHours(rrd_archive::eRraAVERAGE, 0.5, 3600, 14 * 24);
	pRra->add_RraHours(rrd_archive::eRraMIN, 0.5, 3600, 14 * 24);
	pRra->add_RraHours(rrd_archive::eRraMAX, 0.5, 3600,14 * 24);

	pRra->create_Rra(false);

	MainThread* pMainThread = new MainThread(pData);
	pMainThread->start();

	std::string psUpdate[2];
	char buf[256];

	while( 0 == pData->GetError() )
	{
		usleep(500);
		ret = pTempSensor->ReadTemp(Temp);
		if(ret)
		{
			printf("Error reading Temperatur\n");
		}
		else
		{
			pData->SetTemperatuer(0,Temp);
			//printf("Aktuelle Temperatur: %g\n",Temp);
		}
		Poti->Measure(MeasuredValue);
		//printf("Poti: %g\n",MeasuredValue);
		pData->SetADC(0,MeasuredValue);
		Optic->Measure(MeasuredValue);
		//printf("Optic: %g\n",MeasuredValue);
		pData->SetADC(1,MeasuredValue);
		//printf("Optic: %g\n",MeasuredValue);
		memset(buf,0,256);
		snprintf(buf,255,"%.4f",Temp);
		psUpdate[0] = buf;
		memset(buf,0,256);
		snprintf(buf,255,"%.4f",MeasuredValue);
		psUpdate[1] = buf;
		pRra->update_RraNow(2,psUpdate);
		Servo1 = pData->GetServo(0);
		Motor_0->SetServo(static_cast<double>(Servo1));

		Motor = pData->GetServo(3);
		Motor_3->SetServo(static_cast<double>(Motor));


	}

	pMainThread->stop();

	delete pMainThread;
	return 0;
}



