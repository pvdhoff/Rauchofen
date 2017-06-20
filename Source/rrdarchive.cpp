/*
 * rrdarchive.cpp
 *
 *  Created on: 15.05.2017
 *      Author: Philipp
 */

#include <iostream>
#include "rrdarchive.h"
#include "rrd.h"
#include <math.h>


std::string rrd_archive::m_sDsTyps[]  = {"GAUGE", "COUNTER", "DERIVE", "DCOUNTER", "DDERIVE", "ABSOLUTE"};
std::string rrd_archive::m_sRraTyps[] = {"AVERAGE", "MIN", "MAX", "LAST"};

rrd_archive::rrd_archive(std::string sFilename, int iStep, bool bDebugOut)
: m_sFilename(sFilename)
, m_iSteps(iStep)
, m_iDs(0)
, m_iRra(0)
, m_DsArgs(new char*[MAX_DS])
, m_RraArgs(new char*[MAX_RRA])
, m_bDebugOut(bDebugOut)
{
	for(int i = 0; i < MAX_DS; i++)
		m_DsArgs[i] = NULL;

	for(int i = 0; i < MAX_RRA; i++)
		m_RraArgs[i] = NULL;
}

rrd_archive::~rrd_archive() {
	for(int i = 0; i < m_iDs; i++)
		delete m_DsArgs[i];
	m_iDs = 0;

	for(int i = 0; i < m_iRra; i++)
		delete m_RraArgs[i];
	m_iRra = 0;
	delete [] m_DsArgs;
	delete [] m_RraArgs;
}
void rrd_archive::add_Ds(std::string sName, int iHartBeat, eDsType Type, double dMin, double dMax)
{
	std::string sDs("DS:");

	sDs += sName + ":";

	int iDsType = (int)Type;
	if(iDsType >= (int)eDsCOUNT)
		iDsType = (int) eDsGAUGE;

	sDs += m_sDsTyps[iDsType] + ":";

	char buffer [50];
	memset(buffer,0,50);
	snprintf (buffer, 49, "%d", iHartBeat);
	sDs += std::string(buffer) + ":";

	memset(buffer,0,50);
	snprintf (buffer, 49, "%.2f", dMin);
	sDs += std::string(buffer) + ":";

	memset(buffer,0,50);
	snprintf (buffer, 49, "%.2f", dMax);
	sDs += std::string(buffer);

	char* arg = new char[sDs.length()+1];
	memset(arg, 0, sDs.length()+1);
	memcpy(arg, sDs.c_str(), sDs.length());
	m_DsArgs[m_iDs] = arg;
	m_iDs++;
}
void rrd_archive::add_RraHours(eRraType Type, double dxff, int iNrPrimery, double dHours)
{
	double dSeconds = dHours * 60 * 60;
	int iRows = ceil(dSeconds / (double) (iNrPrimery * m_iSteps));

	std::string sRRA("RRA:");

	int iRraType = (int)Type;
	if(iRraType >= (int)eRraCOUNT)
		iRraType = (int) eRraAVERAGE;

	sRRA += m_sRraTyps[iRraType] + ":";

	char buffer [50];
	memset(buffer,0,50);
	snprintf (buffer, 49, "%.5f", dxff);
	sRRA += std::string(buffer) + ":";

	memset(buffer,0,50);
	snprintf (buffer, 49, "%d", iNrPrimery);
	sRRA += std::string(buffer) + ":";

	memset(buffer,0,50);
	snprintf (buffer, 49, "%d", iRows);
	sRRA += std::string(buffer);

	char* arg = new char[sRRA.length()+1];
	memset(arg, 0, sRRA.length()+1);
	memcpy(arg, sRRA.c_str(), sRRA.length());
	m_RraArgs[m_iRra] = arg;
	m_iRra++;
}

bool rrd_archive::doesFileExist(const char* filename)
{
  FILE* fptr = fopen(filename, "r");
  if (fptr != NULL)
  {
    fclose(fptr);
    return true;
  }
  return false;
}

int rrd_archive::update_RraNow(int iDs, std::string sValues[])
{
	int iRet = 0;
	if(iDs > m_iDs)
		iDs = m_iDs;
	char* cCreateArgs[4];
	std::string su("rrdupdate");
	cCreateArgs[0] = (char*)su.c_str();
	cCreateArgs[1] = (char*)m_sFilename.c_str();

	std::string sVal("N");
	std::string sU("U");
	for(int i = 0; i < iDs; i++)
		sVal += ":" + sValues[i];

	for(int i = iDs; i < m_iDs; i++)
		sVal += ":" + sU;
	cCreateArgs[2] = (char*)sVal.c_str();
	cCreateArgs[3] = NULL;

	iRet = rrd_update(3, cCreateArgs);

	if( true == m_bDebugOut)
	{
		std::cout << "Args: " << std::endl;
		for(int i = 0; i < 3; i++)
			std::cout << "cCreateArgs[" << i <<"]: " <<  cCreateArgs[i] << std::endl;

		if(iRet != 0)
		{
			std::cout << "rrd_update returns: " << iRet << std::endl;
			std::cout << "rrd_error_string: " <<  rrd_get_error() << std::endl;
		}
	}

	return iRet;
}

int  rrd_archive::create_Rra(bool bOverideRra)
{
	int iRet = 0;
	bool bDoCreate = true;
	bool bFileExists = doesFileExist(m_sFilename.c_str());
	if(true == bFileExists && false == bOverideRra)
		bDoCreate = false;

	if(true == bDoCreate)
	{
		char bufferStep [50];
		memset(bufferStep,0,50);
		int iNrArg = m_iDs + m_iRra;
		char** cCreateArgs = new char*[iNrArg + 4];
		std::string sc("rrdcreate");
		cCreateArgs[0] = (char*)sc.c_str();
		cCreateArgs[1] = (char*)m_sFilename.c_str();
		snprintf(bufferStep, 49, "--step=%d",m_iSteps);
		cCreateArgs[2] = bufferStep;

		for(int i = 0; i < m_iDs; i++)
		{
			cCreateArgs[3 + i] = m_DsArgs[i];
		}

		for(int i = 0; i < m_iRra; i++)
		{
			cCreateArgs[3 + m_iDs + i] = m_RraArgs[i];
		}

		optind = opterr = 0;
		rrd_clear_error();
		iRet = rrd_create(iNrArg + 3, cCreateArgs);

		if( true == m_bDebugOut)
		{
			std::cout << "Args: " << std::endl;
			for(int i = 0; i < iNrArg + 3; i++)
				std::cout << "cCreateArgs[" << i <<"]: " <<  cCreateArgs[i] << std::endl;

			if(iRet != 0)
			{
				std::cout << "rrd_create returns: " << iRet << std::endl;
				std::cout << "rrd_error_string: " <<  rrd_get_error() << std::endl;
			}
		}

		delete [] cCreateArgs;
	}
	return iRet;
}
