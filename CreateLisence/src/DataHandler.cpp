#include "DataHandler.h"
#include <fstream>
string g_strPublicKeyHexData = "877D7D6AC26548622BBB5A8BC68DFCD059A61B351464B228DB81781910CD3E51";
string g_strSecretKeyHexData = "56ADDDDA7133C6DB2DBA9B74433FF5AA79AE0A32EDB8E8EFE482095A59FCBDA7877D7D6AC26548622BBB5A8BC68DFCD059A61B351464B228DB81781910CD3E51";

#include "date.h"

CDataHandler::CDataHandler():
	m_nLicenseDueData(0),
	m_nLicense(0)
{

}


CDataHandler::~CDataHandler()
{
}

int CDataHandler::handle(int argc, char* argv[])
{

	if (argc == 1)
	{
		// 显示数据	
		return InitLicenseInfo();
	}
	else if(argc ==3 &&  0 == strcmp("-r", argv[1]) )
	{
		// 显示数据
		string strFilePath(argv[2]);
		return InitLicenseInfo(strFilePath);
	}
	else if (argc == 3 && 0 == strcmp("-w", argv[1]) )
	{
		string strDate(argv[2]);
		if (strDate.size()!=8)
		{
			cout << "时间格式错误，格式为：20170920，八位数字" << endl;
			return -1;
		}
		int ndate = atoi(strDate.c_str());
		return WriteLisence(ndate);
	}
	else if (argc == 3 && 0 == strcmp("-d", argv[1]))
	{
		int ndate = atoi(argv[2]);
		return WriteLisence(ndate,1);
	}
	else if (argc == 3 && 0 == strcmp("-m", argv[1]))
	{
		int ndate = atoi(argv[2]);
		return WriteLisence(ndate,2);
	}
	else if (argc == 3 && 0 == strcmp("-y", argv[1]))
	{
		string strDate(argv[2]);
		if (strDate.size() != 4)
		{
			cout << "时间格式错误，格式为：2017，四位数字" << endl;
			return -1;
		}
		int ndate = atoi(strDate.c_str());
		return WriteLisence(ndate,3);
	}
	else
	{
		cout << "不支持的参数" << endl;
		return -1;
	}
	

	return 0;
}

int CDataHandler::InitLicenseInfo(string strFilePath)
{

	cout << "----------------------------------Licence info-------------------------" << endl;
	//License;
	int nRes = 0;
	char * pLicenseData = NULL;
	string strLicPath = "./License.dat";
	if (!strFilePath.empty())
	{
		strLicPath = strFilePath;
	}
	int nLen;
	string strDestHexData = GetLicenseData(strLicPath);
	if (strDestHexData.empty())
	{
		cout << "License 文件不存在！" << endl;
		m_nLicense = -1;
	}
	if (m_nLicense == LIC_SUCCESS)
	{
		m_nLicense = GetDecodeData(g_strPublicKeyHexData.c_str(), g_strPublicKeyHexData.size(), strDestHexData.c_str(), strDestHexData.size(), pLicenseData, nLen);
	}

	if (m_nLicense == LIC_SUCCESS)
	{
		//
		if (pLicenseData)
		{
			string strLicDate(pLicenseData, 8);
			string strLaskUpdatetime(pLicenseData + 8, nLen - 8);
			int nLicDate = atoi(strLicDate.c_str());
			unsigned long ulLastUpdateTime = atol(strLaskUpdatetime.c_str());
			m_nLicenseDueData = nLicDate;
			LicenseRelease(pLicenseData);
			time_t tCur;
			time(&tCur);
			// get current time
			unsigned long ulCurTime = (unsigned long)tCur;
			if (ulCurTime < ulLastUpdateTime)
			{
	
				nRes = m_nLicense;
			}
			else
			{
				long lCurDate = GetDate();
				if (lCurDate > nLicDate)
				{
					cout << "License 过期, EndDate：" << nLicDate << endl;

				}
				else
				{					
					cout << "License EndDate：" << nLicDate << endl;
					cout << "License Update time:" << ulLastUpdateTime << endl;
					int nDayCount = GetDayCount(lCurDate, nLicDate);
					if (nDayCount < 30)
					{
						cout << "license 认证许可时间，剩余 " << nDayCount << " 天" << endl;
					}			

// 					char* pDestHexData = NULL;
// 					int nDestHexDataLen = 0;
					//更新License文件
					char buf[100] = { 0 };
					memset(buf, 0, 100);
					sprintf(buf, "%s%ld", strLicDate.c_str(), (unsigned long)tCur);
					string strSrcData(buf);
					nRes = WriteLisenceData(strSrcData, strLicPath);
// 					nRes = GetEncodeHexData(g_strSecretKeyHexData.c_str(), g_strSecretKeyHexData.size(), strSrcData.c_str(), strSrcData.size(), strLicPath.c_str(), strLicPath.size(), pDestHexData, nDestHexDataLen);
// 					LicenseRelease(pDestHexData);
// 					if (nRes != LIC_SUCCESS)
// 					{
// 						// 更新错误
// 						cout << "License update error! " << strSrcData << endl;		
// 						m_nLicense = -1;
// 					}
				}
			}

		}
		else
		{
			cout << "license data NULL" << endl;
			m_nLicense = -1;
		}
	}
	else
	{
		cout << "license fail code:" << m_nLicense << endl;
		m_nLicense = -1;
	}
	cout << "----------------------------------Licence info-------------------------" << endl;
	return nRes;
}

string CDataHandler::GetLicenseData(string strFilePath)
{
	std::string strLicData;
	ifstream fin;
	fin.open(strFilePath.c_str());
	if (!fin.is_open())
	{
		fin.close();
		return "";
	}
	char buf[1024];
	memset(buf, 0, 1024);
	int nLen = 0;
	while (!fin.eof())
	{
		fin.read(buf, 1024);
		nLen = fin.gcount();
		for (int i = 0; i < nLen; i++)
		{
			strLicData.push_back(buf[i]);
		}
		memset(buf, 0, 1024);
	}
	fin.close();

	return strLicData;
}

time_t CDataHandler::Convertinttotime_t(int nDate)
{
	tm info = { 0 };
	info.tm_year = nDate / 10000 - 1900;
	info.tm_mon = (nDate % 10000) / 100 - 1;
	info.tm_mday = nDate % 100;
	return mktime(&info);
}

int CDataHandler::GetDayCount(int from, int to)
{
	if (from > to)
	{
		return 0;
	}
	int fromSecond = Convertinttotime_t(from);
	int toSecond = Convertinttotime_t(to);
	int nDayCount = (toSecond - fromSecond) / 24 / 3600 + 1;
	return nDayCount;
}

long CDataHandler::GetDate()
{
	long lDate;

	time_t t;
	struct tm * a;
	time(&t);
	a = localtime(&t);

	lDate = (a->tm_year + 1900) * 10000 + (a->tm_mon + 1) * 100 + a->tm_mday;


	return lDate;
}

int CDataHandler::WriteLisenceData(string strData, string strFilePath )
{
	int nRes = 0;
	char* pDestHexData = NULL;
	int nDestHexDataLen = 0;
	nRes = GetEncodeHexData(g_strSecretKeyHexData.c_str(), g_strSecretKeyHexData.size(), strData.c_str(), strData.size(), strFilePath.c_str(), strFilePath.size(), pDestHexData, nDestHexDataLen);
	LicenseRelease(pDestHexData);
	if (nRes != LIC_SUCCESS)
	{
		// 更新错误
		cout << "License update error! " << strData << endl;
		m_nLicense = -1;
	}
	return nRes;
}

// ntype 0: YYYYMMDD
// 1: + nD
// 2: + nM
// 3: + nY
int CDataHandler::WriteLisence(int ndate, int nType, string strFilePath )
{
	int nRes = 0;
	
	long lCurDate = GetDate();
	char buf[100] = { 0 };
	memset(buf, 0, 100);
	time_t tCur;
	time(&tCur);
	
	if (nType == 0 )
	{
		sprintf(buf, "%d%ld", ndate, (unsigned long)tCur);
	}else if (nType == 1 )
	{

	}
	else if (nType == 2)
	{

	}
	else if (nType == 3)
	{

	}
	
	//TODO: check date

	string strData(buf);
	nRes = WriteLisenceData(strData, strFilePath);

	return nRes;
}