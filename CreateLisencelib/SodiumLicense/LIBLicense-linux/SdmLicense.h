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
	 // ����֤��
	int EncodeLicense(const char* pDate,const int nLen);
	//����֤��
	int DecodeLicense(std::string & strData); 
	// ���ɹ�Կ˽Կ
	int GeneratePubPriKey();

	string ConvertStringToHexData(string str);
	string ConvertHexDataToString(string strHexData);
	
	int EncodeLicense(const string strSKData,const string strSrcData,const string strWFileName ,string& strDestHexData);
	int DecodeLicense(const string strPKdata,const string strDestData ,string &strSrcData);
	int SaveHexDataToFile(const string strFileName,const string strData);
	//Function
private:
	// ��ʼ��һ�鹫Կ˽Կ nRes: 0: �ɹ� ��0: ʧ��
	int InitSdmLicenseKey(std::string & strpublicKey, std::string & strprivateKey);
	bool isExistFile(const std::string& strFilePath);
	// �������ݵ��ļ� DataType 0: ˽Կ 1����Կ 2��ǩ������
	bool SaveDataToFile(const std::string& strData,const int nDataType);
	// ��ȡ�ļ� DataType 0: ˽Կ 1����Կ 2��ǩ������
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