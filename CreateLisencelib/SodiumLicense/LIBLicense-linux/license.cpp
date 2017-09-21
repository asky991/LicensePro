#include "stdafx.h"
#ifdef WIN32
#include "../License/LicDefine.h"
#include "../License/license.h"
#include "../License/SdmLicense.h"
#else
#include "LicDefine.h"
#include "license.h"
#include "SdmLicense.h"
#endif

#include "NtpService.h"

extern char g_strNTPServiceAddr[50];
// 初始化IP地址
void InitNTPServiceAddr(char*pAddr,int nLen)
{
	if (!pAddr)
	{
		return ;
	}
	memset(g_strNTPServiceAddr,0,50);
	memcpy(g_strNTPServiceAddr,pAddr,nLen);
}


// 生成公钥私钥
int GeneratePubPriKey()
{

	remove(PUBLICKEYPATH);
	remove(PRIVATEKEYPATH);
	remove(LICENSEDATPATH);

	CSdmLicense Lic;
	int nRes=Lic.GeneratePubPriKey();
	return nRes;
}


int EncodeLicense(const char* pDate,const int nLen)
{
	CSdmLicense Lic;
	int nRes=Lic.EncodeLicense(pDate,nLen);
	return nRes;
}
//解密证书
int DecodeLicense(char* &pData,int &nLen)
{
	CSdmLicense Lic;
	std::string strData;
	int nRes=Lic.DecodeLicense(strData);
	nLen = strData.size();
	if (nLen)
	{
		pData=new char[nLen+1];
		memset(pData,0,nLen+1);
		sprintf(pData,"%s",strData.c_str());
	}

	return nRes;
}

void LicenseRelease(char* & pData)
{
	if (pData)
	{
		delete[] pData;
		pData=NULL;
	}
}
int GetDatatimeFromNTFService(int& nDate,int& nTime)
{

	CNtpService ntp;
	int nRes= ntp.GetDataTimeFromNTPService(nDate,nTime);

	return nRes;
}
int GetEncodeHexData(const char* pHexSK,const int nHexSKlen,const char* pSrcData,const int nSrcDataLen,const char* pLicensePath,const int nPathLen,char* &pDestHexData,int& nDestHexLen)
{
	int nRes=0;
	if (!pHexSK || nHexSKlen==0 || !pSrcData || nSrcDataLen==0 )
	{
		return LIC_PARAM_ERROR;
	}
	string strSKHexData(pHexSK,nHexSKlen),strSKData,strSrcData(pSrcData,nSrcDataLen),strDestHexData;
	string strLicensePath(pLicensePath,nPathLen);

	CSdmLicense Lic;
	strSKData=Lic.ConvertHexDataToString(strSKHexData);
	nRes = Lic.EncodeLicense(strSKData,strSrcData,strLicensePath,strDestHexData);
	if (nRes == LIC_SUCCESS)
	{
		nDestHexLen = strDestHexData.size();
		pDestHexData=new char[nDestHexLen+1];
		memset(pDestHexData,0,nDestHexLen+1);
		memcpy(pDestHexData,strDestHexData.c_str(),nDestHexLen);
	}

	return nRes;
}
int GetDecodeData(const char* pHexPK,const int nHexPKlen,const char* pDestData,const int nDestDataLen,char* &pSrcData,int& nSrcLen)
{
	int nRes=0;
	if (!pHexPK || nHexPKlen==0 || !pDestData || nDestDataLen==0 )
	{
		return LIC_PARAM_ERROR;
	}
	string strHexPK(pHexPK,nHexPKlen),strDestHexData(pDestData,nDestDataLen),strSrcHexData;
	string strPK,strDest,strSrc;
	CSdmLicense Lic;
	strPK = Lic.ConvertHexDataToString(strHexPK);
	strDest=Lic.ConvertHexDataToString(strDestHexData);
	nRes = Lic.DecodeLicense(strPK,strDest,strSrc);
	if (nRes==LIC_SUCCESS)
	{
		nSrcLen=strSrc.size();
		pSrcData = new char[nSrcLen+1];
		memset(pSrcData,0,nSrcLen+1);
		memcpy(pSrcData,strSrc.c_str(),nSrcLen);
	}

	return nRes;
}