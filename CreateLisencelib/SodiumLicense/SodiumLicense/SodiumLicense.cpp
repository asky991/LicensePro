// SodiumLicense.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define MESSAGE (const unsigned char *) "test"
#define MESSAGE_LEN 4
#include "../License/SdmLicense.h"
int _tmain(int argc, _TCHAR* argv[])
{
	int nRes=0;
	
	
	//remove("./public.bin");
	//remove("./private.bin");
	//remove("./license.dat");


	CSdmLicense sdmLic;
	nRes=sdmLic.GeneratePubPriKey();
	cout<<"GeneratePPkey Res :"<<nRes<<endl;

	string Src="test",strDest;
	nRes = sdmLic.EncodeLicense(Src.c_str(),Src.length());
	cout<<"Encode Res :"<<nRes<<endl;
	nRes = sdmLic.DecodeLicense(strDest);
	cout<<"Decode Res :"<<nRes<<strDest<<endl;


	cout<<"end"<<endl;
	getchar();
	return 0;
	
// 	std::string data="test";
// 	std::stringstream ss;
// 	for(int i=0;i<data.size();i++)
// 	{
// 		ss<<std::hex<<std::setw(2)<<(int)(unsigned char)data.at(i);
// 	}
// 	std::string strHex=ss.str();
	// 签名
	unsigned char pk[crypto_sign_PUBLICKEYBYTES];
	unsigned char sk[crypto_sign_SECRETKEYBYTES];
	nRes =crypto_sign_keypair(pk, sk); // 随机产生一堆公钥和秘钥

	cout<<"PK: "<<pk<<" "<<crypto_sign_PUBLICKEYBYTES<< endl;
	cout<<"SK: "<<sk<<" "<<crypto_sign_SECRETKEYBYTES<<endl;


	unsigned char signed_message[crypto_sign_BYTES + MESSAGE_LEN];
	unsigned long long signed_message_len;

	nRes =crypto_sign(signed_message, &signed_message_len,
		MESSAGE, MESSAGE_LEN, sk); // 秘钥对数据签名



	// 验证
	unsigned char unsigned_message[MESSAGE_LEN];
	unsigned long long unsigned_message_len;
	if (crypto_sign_open(unsigned_message, &unsigned_message_len,  // 用公钥验证签名
		signed_message, signed_message_len, pk) != 0) {
			/* Incorrect signature! */
			cout<<unsigned_message<< unsigned_message_len<<endl;
	}else
	{
		
		cout<< "En："<<signed_message<< signed_message_len<<endl;
		cout<< "DE："<<unsigned_message<< unsigned_message_len<<endl;
	}

	cout<<"end"<<endl;
	getchar();
	
	return 0;
}

