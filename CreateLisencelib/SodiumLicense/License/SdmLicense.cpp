
//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <string.h>
using namespace std;
#include "SdmLicense.h"
#include "LicDefine.h"
#ifdef WIN32
#include <io.h>
#else
#include <sys/io.h>
 #include<unistd.h>
#endif


#include <iostream>
#include <string.h>
#include<fstream>
#include<iomanip>
using namespace std;
#include <sstream> 
#include <iomanip>

#include "../libsodium-master/src/libsodium/include/sodium.h"
//#include "sodium.h"
#pragma comment(lib,"libsodium.lib")



CSdmLicense::CSdmLicense()
{
	m_privateKey.clear();
	m_publicKey.clear();
}

CSdmLicense::~CSdmLicense()
{

}
// 生成公钥私钥
int  CSdmLicense::GeneratePubPriKey()
{
	int nRes=LIC_SUCCESS;
	bool bRes=false;
	if (!m_publicKey.empty() && isExistFile(PUBLICKEYPATH))
	{
		return LIC_PUBLICKEYEXIST;
	}
	if (!m_privateKey.empty() && isExistFile(PRIVATEKEYPATH))
	{
		return LIC_PRIVATEKEYEXIST;
	}
	nRes= InitSdmLicenseKey(m_publicKey,m_privateKey);
	if (nRes)
	{
		return LIC_GENERATEWKEYFAIL;
	}
	bRes = SaveDataToFile(m_privateKey,0);

	bRes = SaveDataToFile(m_publicKey,1);

	return nRes;
}
// 生成证书 用私钥
int CSdmLicense::EncodeLicense(const char* pDate,const int nLen)
{
	int nRes = LIC_SUCCESS;
	unsigned char sk[crypto_sign_SECRETKEYBYTES];
	if (!pDate || nLen<1)
	{
		return LIC_PARAM_ERROR;
	}

	m_privateKey.clear();
	m_publicKey.clear();
	m_licenseDat.clear();
	if (m_privateKey.empty())
	{
		if (!isExistFile(PRIVATEKEYPATH))
		{
			return LIC_PRIVATEKEYNOEXIST;
		}
		// 读取私钥
		LoadDataFromFile(m_privateKey,0);
	}
	memcpy(sk,m_privateKey.c_str(),crypto_sign_SECRETKEYBYTES);
	if (isExistFile(LICENSEDATPATH))
	{
		return LIC_LICENSEDATEXIST;
	}
	unsigned char* psigned_data= new unsigned char[crypto_sign_BYTES + nLen];
	memset(psigned_data,0,crypto_sign_BYTES + nLen);
	unsigned long long signed_data_len;

	unsigned char* punData=new unsigned char[nLen];
	memcpy(punData,pDate,nLen);
	nRes =crypto_sign(psigned_data, &signed_data_len,punData, nLen, sk); // 秘钥对数据签名
	if (!nRes)
	{
		// 写License.dat 文件
		std::string strLicenseData;
		char data;
		for(int i=0;i<signed_data_len;i++)
		{
			data=psigned_data[i];
			strLicenseData.push_back(data);
		}
		m_licenseDat=strLicenseData;
		bool bRes = SaveDataToFile(strLicenseData,2);
	}else{

		nRes= LIC_SIGN_ERROR;
	}

	delete [] psigned_data;
	psigned_data = NULL;
	delete [] punData;
	punData = NULL;
	return nRes;
}
//解密证书 用公钥
int CSdmLicense::DecodeLicense(std::string & strData)
{
	int nRes=LIC_SUCCESS;
	unsigned char pk[crypto_sign_PUBLICKEYBYTES];
	m_privateKey.clear();
	m_publicKey.clear();
	m_licenseDat.clear();
	if (m_publicKey.empty())
	{
		if (!isExistFile(PUBLICKEYPATH))
		{
			return LIC_PUBLICKEYNOEXIST;
		}
		LoadDataFromFile(m_publicKey,1);
	}
	
	memcpy(pk,m_publicKey.c_str(),crypto_sign_PUBLICKEYBYTES);
	std::string strLicenseData=m_licenseDat;
	strLicenseData.clear();
	if (strLicenseData.empty())
	{
		if (!isExistFile(LICENSEDATPATH))
		{
			return LIC_LICENSEDATNOEXIST;
		}

		LoadDataFromFile(strLicenseData,2);
	}

	unsigned char* psigned_message=new unsigned char[strLicenseData.size()];
	memcpy(psigned_message,strLicenseData.c_str(),strLicenseData.size());
	unsigned long long smlen=strLicenseData.size();

	int nunsignData=smlen - crypto_sign_SECRETKEYBYTES + 10;
	unsigned long long mlen_p;
	unsigned char* punsigned_message=new unsigned char[nunsignData];
	memset(punsigned_message,0,nunsignData); 

	if (crypto_sign_open(punsigned_message, &mlen_p,  // 用公钥验证签名
		psigned_message, smlen, pk) != 0) 
	{
			nRes=LIC_SIGNOPEN_ERROR;
	}else
	{
		strData.clear();
		char buf;
		for(int i=0;i<mlen_p;i++)
		{
			buf=punsigned_message[i];
			strData.push_back(buf);
		}
	}

	return nRes;
}



int CSdmLicense::InitSdmLicenseKey(std::string & strpublicKey, std::string & strprivateKey)
{
	unsigned char pk[crypto_sign_PUBLICKEYBYTES];
	unsigned char sk[crypto_sign_SECRETKEYBYTES];
	char ppk[crypto_sign_PUBLICKEYBYTES];
	char ssk[crypto_sign_SECRETKEYBYTES];
	int nRes=crypto_sign_keypair(pk, sk); // 随机产生一堆公钥和秘钥
	memcpy(ppk,pk,crypto_sign_PUBLICKEYBYTES);
	memcpy(ssk,sk,crypto_sign_SECRETKEYBYTES);
	std::string strpk(ppk,crypto_sign_PUBLICKEYBYTES);
	std::string strsk(ssk,crypto_sign_SECRETKEYBYTES);
	strpublicKey = strpk;
	strprivateKey = strsk;	
	return nRes;
}

bool CSdmLicense::isExistFile(const std::string& strFilePath)
{
	return  (access(strFilePath.c_str(), 0) == 0);
}

// 保存数据到文件 DataType 0: 私钥 1：公钥 2：签名数据
bool CSdmLicense::SaveDataToFile(const std::string& strData,int nDataType)
{
	std::vector<unsigned char> vec;
	std::string strFilePath;
	if (nDataType==0)
	{
		strFilePath = PRIVATEKEYPATH;
	}else if (nDataType ==1 )
	{
		strFilePath =PUBLICKEYPATH;
	}else if (nDataType==2)
	{
		strFilePath = LICENSEDATPATH;
	}else
	{
		return false;
	}

	SaveDataToFile(strFilePath,strData);
	return true;
}


// 读取文件 DataType 0: 私钥 1：公钥 2：签名数据
bool CSdmLicense::LoadDataFromFile(std::string& strData,const int nDataType)
{
	std::vector<unsigned char> vec;
	std::string strFilePath;
	if (nDataType==0)
	{
		strFilePath = PRIVATEKEYPATH;
	}else if (nDataType ==1 )
	{
		strFilePath =PUBLICKEYPATH;
	}else if (nDataType==2)
	{
		strFilePath = LICENSEDATPATH;
	}else
	{
		return false;
	}

	LoadDataFromFile(strFilePath,strData);
	return true;
}





void CSdmLicense::SaveDataToFile(const std::string& fileName, const std::string& data)
{
	
	int nHexLen=data.size()*2+1;
	char *pHex=new char[nHexLen];
	memset(pHex,0,nHexLen);
	for (int i=0;i<nHexLen/2;i++)
	{
		sprintf(pHex,"%s%02X",pHex,(unsigned char)data.at(i));
	}
	ofstream file;
	file.open(fileName.c_str());
	file.write(pHex,nHexLen-1);	
	file.close();//关闭文件

	if (pHex)
	{
		//cout<<"save name :"<<fileName<<" count: "<<nHexLen<<"data :"<< pHex<< endl;
		delete[] pHex;
		pHex = NULL;
	}else
	{
		cout<<__FUNCTION__<<"new fail"<<endl;
	}

}

void CSdmLicense::LoadDataFromFile(const std::string& fileName, std::string& data)
{
	ifstream fin;
	fin.open(fileName.c_str());
	if (!fin)
	{
		fin.close();
		return ;
	}
	char buf[1024];
	memset(buf,0,1024);
	unsigned char tounchar;
	int nLen=0;
	while(!fin.eof())
	{
		fin.read(buf,1024);
		nLen = fin.gcount();
		for (int i=0;i<nLen;i+=2)
		{
			tounchar=HextoNum(buf[i])*16+HextoNum(buf[i+1]);
			data.push_back(tounchar);
		}

	}
	fin.close();
	//cout<<"load name :"<<fileName<<" count: "<<nLen<<"data :"<< buf<< endl;
}

int CSdmLicense::HextoNum(char hex)
{
	if (hex>58)
	{
		return hex-55;
	}else
	{
		return hex-48;
		
	}
}

string CSdmLicense::ConvertStringToHexData(string str)
{
	

	int nHexLen=str.size()*2+1;
	char *pHex=new char[nHexLen];
	memset(pHex,0,nHexLen);
	for (int i=0;i<nHexLen/2;i++)
	{
		sprintf(pHex,"%s%02X",pHex,(unsigned char)str.at(i));
	}
	string strHexData(pHex,nHexLen-1);
	return strHexData;
}
string CSdmLicense::ConvertHexDataToString(string strHexData)
{
	string str;
	unsigned char tounchar;
	char c1,c2;
	for(int i=0;i<strHexData.size()/2;i++)
	{
		c1=strHexData.at(i*2);
		c2= strHexData.at(i*2+1);
		tounchar=HextoNum(c1)*16 + HextoNum(c2);
		str.push_back(tounchar);
	}
	return str;
}

int CSdmLicense::EncodeLicense(const string strSKData,const string strSrcData,const string strWFileName ,string& strDestHexData)
{
	
	int nRes = LIC_SUCCESS;
	unsigned char sk[crypto_sign_SECRETKEYBYTES];
	memcpy(sk,strSKData.c_str(),crypto_sign_SECRETKEYBYTES);

	int nSrcDataLen=strSrcData.size();
	unsigned char* psigned_data= new unsigned char[crypto_sign_BYTES + nSrcDataLen];
	memset(psigned_data,0,crypto_sign_BYTES + nSrcDataLen);
	unsigned long long signed_data_len;

	unsigned char* punData=new unsigned char[nSrcDataLen];
	memcpy(punData,strSrcData.c_str(),nSrcDataLen);
	nRes =crypto_sign(psigned_data, &signed_data_len,punData, nSrcDataLen, sk); // 秘钥对数据签名
	if (!nRes)
	{

		char data;
		for(int i=0;i<signed_data_len;i++)
		{
			data=psigned_data[i];
			strDestHexData.push_back(data);
		}
		strDestHexData=ConvertStringToHexData(strDestHexData);
		if (!strWFileName.empty())
		{
			if (isExistFile(strWFileName))
			{
				remove(strWFileName.c_str());
			}
			nRes = SaveHexDataToFile(strWFileName,strDestHexData);
		}

		
	}else{

		nRes= LIC_SIGN_ERROR;
	}

	delete [] psigned_data;
	psigned_data = NULL;
	delete [] punData;
	punData = NULL;
	return nRes;
	
}

int CSdmLicense::DecodeLicense(const string strPKdata,const string strDestData ,string &strSrcData)
{

	int nRes=LIC_SUCCESS;
	unsigned char pk[crypto_sign_PUBLICKEYBYTES];
	memcpy(pk,strPKdata.c_str(),crypto_sign_PUBLICKEYBYTES);

	unsigned char* psigned_message=new unsigned char[strDestData.size()];
	memcpy(psigned_message,strDestData.c_str(),strDestData.size());
	unsigned long long smlen=strDestData.size();

	int nunsignData=smlen - crypto_sign_SECRETKEYBYTES + 10;
	unsigned long long mlen_p;
	unsigned char* punsigned_message=new unsigned char[nunsignData];
	memset(punsigned_message,0,nunsignData); 

	if (crypto_sign_open(punsigned_message, &mlen_p,  // 用公钥验证签名
		psigned_message, smlen, pk) != 0) 
	{
		nRes=LIC_SIGNOPEN_ERROR;
	}else
	{
		strSrcData.clear();
		char buf;
		for(int i=0;i<mlen_p;i++)
		{
			buf=punsigned_message[i];
			strSrcData.push_back(buf);
		}
	}

	return nRes;
}

int CSdmLicense::SaveHexDataToFile(const string strFileName,const string strData)
{
	int nRes = LIC_SUCCESS;
	ofstream file;
	file.open(strFileName.c_str());
	if (file.is_open())
	{
		file.write(strData.c_str(),strData.size());	
		file.close();//关闭文件
	}else
	{
		nRes = LIC_FILEOPENFAIL_ERROR;
	}
	return nRes;
}

