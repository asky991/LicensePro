#ifndef __STDLICENSE_H__
#define __STDLICENSE_H__

#define PUBLICKEYPATH		"./public.bin"
#define PRIVATEKEYPATH		"./private.bin"
#define LICENSEDATPATH		"./license.dat"

#include <vector>

class CSdmLicense
{


public:
	CSdmLicense();
	~CSdmLicense();
	 // 生成证书
	int EncodeLicense(const char* pDate,const int nLen);
	//解密证书
	int DecodeLicense(std::string & strData); 
	// 生成公钥私钥
	int GeneratePubPriKey();

	string ConvertStringToHexData(string str);
	string ConvertHexDataToString(string strHexData);
	
	int EncodeLicense(const string strSKData,const string strSrcData,const string strWFileName ,string& strDestHexData);
	int DecodeLicense(const string strPKdata,const string strDestData ,string &strSrcData);
	int SaveHexDataToFile(const string strFileName,const string strData);
	//Function
private:
	// 初始化一组公钥私钥 nRes: 0: 成功 非0: 失败
	int InitSdmLicenseKey(std::string & strpublicKey, std::string & strprivateKey);
	bool isExistFile(const std::string& strFilePath);
	// 保存数据到文件 DataType 0: 私钥 1：公钥 2：签名数据
	bool SaveDataToFile(const std::string& strData,const int nDataType);
	// 读取文件 DataType 0: 私钥 1：公钥 2：签名数据
	bool LoadDataFromFile(std::string& strData,const int nDataType);


	void SaveDataToFile(const std::string& fileName, const std::string& data);
	void LoadDataFromFile(const std::string& fileName, std::string& data);
	int HextoNum(char hex);

	//Param
private:
	std::string m_publicKey;
	std::string m_privateKey;
	std::string m_licenseDat;
	
};

#endif