// LibLicenseTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") 

#include "../License/LicDefine.h"
#include "../License/license.h"
#pragma comment(lib,"LIBLicense.lib")

void update();

int _tmain(int argc, _TCHAR* argv[])
{

	std::string strSrc="20160917";
	int nRes=0;

	time_t t;
	time(&t);
	char buf[100]={0};
	sprintf(buf,"%s%ld",strSrc.c_str(),(unsigned long)t);
	cout<<buf<<endl;


// 	nRes=GeneratePubPriKey();
// 	cout<<"Generate Res:"<<nRes<<endl;
//	nRes = EncodeLicense(strSrc.c_str(),strSrc.size());
// 	cout<<"EnCode Res:"<<nRes<<endl;
// 	char * pDest=NULL;
// 	int nLen;
// 	nRes = DecodeLicense(pDest,nLen);
// 	cout<<"Decode Res:"<<nRes<<endl;
// 	if (pDest)
// 	{
// 		cout<<pDest<<" : "<<nLen<<endl;
// 		DecodeLicenseRelease(pDest);
// 	}


// 	int nDate=0,nTime=0;
// 	GetDatatimeFromNTFService(nDate,nTime);
// 	cout<<"Date : "<<nDate<<" Time: "<<nTime<<endl;
// 	//update();
	string strPKHexData="877D7D6AC26548622BBB5A8BC68DFCD059A61B351464B228DB81781910CD3E51";
	string strSKHexData="56ADDDDA7133C6DB2DBA9B74433FF5AA79AE0A32EDB8E8EFE482095A59FCBDA7877D7D6AC26548622BBB5A8BC68DFCD059A61B351464B228DB81781910CD3E51";
	string strSrcData(buf);
	string strLicPath="./license.dat";
	char * pDesHexData=NULL;
	int nDestHexDatalen=0;
	nRes =GetEncodeHexData(strSKHexData.c_str(),strSKHexData.size(),strSrcData.c_str(),strSrcData.size(),strLicPath.c_str(),strLicPath.size(),pDesHexData,nDestHexDatalen);
	if (nRes==LIC_SUCCESS)
	{
		cout<<"Encode :"<<pDesHexData<<" len:"<<nDestHexDatalen<<endl;
	}
	char *pSrcData=NULL;
	int nSrcLen=0;
	nRes= GetDecodeData(strPKHexData.c_str(),strPKHexData.size(),pDesHexData,nDestHexDatalen,pSrcData,nSrcLen);
	if (nRes==LIC_SUCCESS)
	{
		cout<<"Decode :"<<pSrcData<<" len :"<<nSrcLen<<endl;
	}
	LicenseRelease(pDesHexData);
	LicenseRelease(pSrcData);

	cout<<"end";
	getchar();

	return 0;
}
 #include <winsock.h>
void update()
{
	//char *hostname=(char *)"163.117.202.33";
	//char *hostname=(char *)"pool.ntp.br";
	char    *hostname=(char *)"200.20.186.76";
	int portno=123;     //NTP is port 123
	int maxlen=1024;        //check our buffers
	int i;          // misc var i
	unsigned char msg[48]={010,0,0,0,0,0,0,0,0};    // the packet we send
	unsigned long  buf[1024]; // the buffer we get back
	memset(buf,0,1024);
	//struct in_addr ipaddr;        //  
	struct protoent *proto;     //
	struct sockaddr_in server_addr;
	int s;  // socket
	long tmit;   // the time -- This is a time_t sort of

	;
	//#we use the system call to open a UDP socket
	//socket(SOCKET, PF_INET, SOCK_DGRAM, getprotobyname("udp")) or die "socket: $!";
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		cout<<"error"<<endl;
	}
	proto=getprotobyname("udp");
	s=socket(PF_INET, SOCK_DGRAM, proto->p_proto);
	perror("socket1");
	//
	//#convert hostname to ipaddress if needed
	//$ipaddr   = inet_aton($HOSTNAME);
	memset( &server_addr, 0, sizeof( server_addr ));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(hostname);
	//argv[1] );
	//i   = inet_aton(hostname,&server_addr.sin_addr);
	server_addr.sin_port=htons(portno);
	//printf("ipaddr (in hex): %x\n",server_addr.sin_addr);





	// send the data
	printf("sending data..\n");
	i=sendto(s,(char*)msg,sizeof(msg),0,(struct sockaddr *)&server_addr,sizeof(server_addr));
	perror("sendto");
	// get the data back
	struct sockaddr saddr;
	socklen_t saddr_l = sizeof (saddr);
	i=recvfrom(s,(char*)buf,48,0,&saddr,&saddr_l);
	perror("recvfr:");


	//We get 12 long words back in Network order
	/*
	for(i=0;i<12;i++) {
		//printf("%d\t%-8x\n",i,ntohl(buf[i]));
		long tmit2=ntohl((time_t)buf[i]);
		std::cout << "Round number " << i << " time is " << ctime(&tmit2)  << std::endl;
	}
	*/


	//tmit=ntohl((time_t)buf[10]);    //# get transmit time
	tmit=ntohl((time_t)buf[4]);    //# get transmit time
	//printf("tmit=%d\n",tmit);





	tmit-= 2208988800U; 
	struct tm *local_time = NULL;
	char str_time[100];
	time_t utc_time = tmit; 
	local_time = localtime(&utc_time);
	strftime(str_time, sizeof(str_time), "%Y-%m-%d,%H:%M:%S", local_time);

	printf ("Time: %s/n", str_time);

	//#compare to system time
	//printf("Time: %s",ctime(&tmit));


	//std::cout << "time is " << ctime((time_t *)&tmit)  << std::endl;
	//i=time(0);
	//printf("%d-%d=%d\n",i,tmit,i-tmit);
	//printf("System time is %d seconds off\n",(i-tmit));
	//std::cout << "System time is " << (i-tmit) << " seconds off" << std::endl;
}