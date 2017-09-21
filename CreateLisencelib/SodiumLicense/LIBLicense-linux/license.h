#ifndef __LICENSE_H__
#define __LICENSE_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32
#   if defined LIBLICENSE_STATIC
#       define LIBLICENSE_EXPORT
#   elif defined DLL_EXPORT
#       define LIBLICENSE_EXPORT __declspec(dllexport)
#   else
#       define LIBLICENSE_EXPORT __declspec(dllimport)
#   endif
#else
#   if defined __SUNPRO_C  || defined __SUNPRO_CC
#       define LIBLICENSE_EXPORT __global
#   elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define LIBLICENSE_EXPORT __attribute__ ((visibility("default")))
#   else
#       define LIBLICENSE_EXPORT
#   endif
#endif


	// 初始化网络地址
	LIBLICENSE_EXPORT void InitNTPServiceAddr(char*pAddr,int nLen);
	// 生成公钥私钥
	LIBLICENSE_EXPORT int GeneratePubPriKey();
	// 生成证书
	LIBLICENSE_EXPORT int EncodeLicense(const char* pDate,const int nLen);
	//解密证书
	LIBLICENSE_EXPORT int DecodeLicense(char* &pData,int &nLen); 


	LIBLICENSE_EXPORT int GetDatatimeFromNTFService(int& nDate,int& nTime);
	
	LIBLICENSE_EXPORT int GetEncodeHexData(const char* pHexSK,const int nHexSKlen,const char* pSrcData,const int nSrcDataLen,const char* pLicensePath,const int nPathLen ,char* &pDestHexData,int& nDestHexLen);
	LIBLICENSE_EXPORT int GetDecodeData(const char* pHexPK,const int nHexPKlen,const char* pDestData,const int nDestDataLen,char* &pSrcData,int& nSrcLen);

	// 调用License函数中 含有char* &, 结束用 该函数释放资源
	LIBLICENSE_EXPORT void LicenseRelease(char* & pData);
}

#endif /*__LICENSE_H__*/

