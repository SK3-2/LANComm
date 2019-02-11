#ifndef _LANCOMM_H
#define _LANCOMM_H

#include <unistd.h>
#include <iostream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <vector>
#include <sys/poll.h>
#include <algorithm>
#include <thread>

#define BUFMAX 1024
#define DEVICEMAX 100
#define SOCKETMAX 100

using namespace std;

extern vector<sockaddr_in> deviceInfo;

//class LocalDevice {
//	private:
//		int id;
//		int deviceName;
//		struct sockaddr_in c_addr;
//	public:
//		LocalDevice();
//		LocalDevice(int, int, sockaddr_in);
//		bool IsEqual(const LocalDevice*, const LocalDevice*);
//};

class UDPComm{
	private:
		int sd; //UDP listen socket descriptor
		int status;
		int on=1;
		struct sigaction act;
		struct sockaddr_in s_addr_s;
		struct sockaddr_in s_addr_c;
		struct sockaddr_in c_addr;
		//		vector<LocalDevice> deviceInfo[DEVICEMAX];
		string brdIp;
		string port_udp;
		string port_c;
		char sendBuffer[BUFMAX];
		char recvBuffer[BUFMAX];
	public:
		//UDPComm();
		UDPComm(string,string);
		//~UDPComm();
		void run();
		void replyDaemon();
		int createBrdSocket();
		void setLocalDeviceInfo();
		int checkDeviceInfo();
		void recvMultipleResponse(int);
		int checkDeviceInfo(sockaddr_in);
		void AlarmTimer(int);
};

class TCPComm{
	private:
		int sd;
		struct sockaddr_in connect_addr;
		int connectlen;
		string port;
		char sendBuffer[BUFMAX];
	public:
		TCPComm(string);
		//~TCPCommServer();
		void run();
		void setDeviceIndex(int);
		void connectSocket();
		//void read();
};

class ReplyDaemon{
	private:
		int c_socket;
		char recvBuffer[BUFMAX], buftemp[BUFMAX];
		struct sockaddr_in s_addr_udp, s_addr_tcp, c_addr; //c_addr for UDP response
		string port;
		int sd_udp;
		int sd_tcp;
		struct pollfd sock_pollfd[SOCKETMAX];
		const struct pollfd* pollfd_end = &sock_pollfd[SOCKETMAX-1];
	public:
		ReplyDaemon(string);
		//~ReplyDamon();
		void run();
		int createUDPSocket();
		int createTCPSocket();
		string recvMsg(int);
		void replyDeviceInfo();
		int getEmptyPfdIndex();
		struct pollfd* getNextPollfd(struct pollfd*);
		struct pollfd* getNextReventPoll(struct pollfd*);
		int acceptPollfd(int);
		void register_Pollfd(int);

};

void sigAlarm(int);

#endif
