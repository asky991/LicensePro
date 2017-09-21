#pragma once

#include <time.h>  
#include <string.h>
#include <iostream>
#include <string>
using namespace std;
#include "LicDefine.h"
#include "license.h"

#ifdef WIN32
#include <Windows.h>
#else
#include<sys/time.h>
#include<time.h>
#endif
class CDataHandler
{
public:
	CDataHandler();
	~CDataHandler();

public:
	int handle(int argc, char* argv[]);


	int InitLicenseInfo(string strFilePath = "");
	time_t Convertinttotime_t(int nDate);
	int GetDayCount(int from, int to);
	string GetLicenseData(string strFilePath);
	long GetDate();

private:
	int WriteLisenceData(string strData, string strFilePath ="./License.dat");
	int WriteLisence(int ndate, int nType = 0, string strFilePath= "./License.dat");
private:
	int  m_nLicenseDueData;
	int m_nLicense;
};

