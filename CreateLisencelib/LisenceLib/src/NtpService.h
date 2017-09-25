#pragma once
#ifndef __NTPSERVICE_H__
#define __NTPSERVICE_H__

class CNtpService
{
public:
	CNtpService(void);
	~CNtpService(void);

	int GetDataTimeFromNTPService(int &nDate,int &nTime);
private:
	/*构建NTP协议包*/
	int construct_packet(char *packet);
	int get_ntp_time(int sk, struct addrinfo *addr, struct ntp_packet *ret_time);
	int UpdateNFP(int &nDate,int &nTime);
};

#endif
