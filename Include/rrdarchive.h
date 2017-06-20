/*
 * rrdarchive.h
 *
 *  Created on: 15.05.2017
 *      Author: Philipp
 */

#ifndef RRDARCHIVE_H_
#define RRDARCHIVE_H_
#include <string>

#define MAX_DS 10
#define MAX_RRA 30
class rrd_archive {
public:
	enum eDsType
	{
		eDsGAUGE,
		eDsCOUNTER,
		eDsDERIVE,
		eDsDCOUNTER,
		eDsDDERIVE,
		eDsABSOLUTE,
		eDsCOUNT
	};
	enum eRraType
	{
		eRraAVERAGE,
		eRraMIN,
		eRraMAX,
		eRraLAST,
		eRraCOUNT
	};
	rrd_archive(std::string sFilename, int iStep, bool bDebugOut);
	virtual ~rrd_archive();
	void add_Ds(std::string sName, int iHartBeat, eDsType Type, double dMin, double dMax);
	void add_RraHours(eRraType Type, double dxff, int iNrPrimery, double dHours);
	int  create_Rra(bool bOverideRra);
	int  update_RraNow(int iDs, std::string sValues[]);
private:
	bool doesFileExist(const char* filename);

	std::string m_sFilename;
	int m_iSteps;
	int m_iDs;
	int m_iRra;
	char** m_DsArgs;
	char** m_RraArgs;
	bool   m_bDebugOut;
public:
	static std::string m_sRraTyps[];
	static std::string m_sDsTyps[];
};

#endif /* RRDARCHIVE_H_ */
