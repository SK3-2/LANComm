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
extern string UserID;

class UDPComm{
	private:
		int sd;
		int status;
		string brdIp;
		string port;
		struct sigaction act;
		struct sockaddr_in s_addr;
		struct sockaddr_in c_addr;
		char sendBuffer[BUFMAX];
		char recvBuffer[BUFMAX];
	public:
		UDPComm() = delete;
		UDPComm(string);
		~UDPComm() = default;
		void run();
		int createBrdSocket();
		string getBrdIp();
		void setLocalDeviceInfo();
		void recvMultipleResponse(int);
		int checkDeviceInfo(sockaddr_in);
		int checkUserID(string);
		void AlarmTimer(int);
		void setAlarmInfo();
};

class TCPComm{
	private:
		int sd;
		int select_num;
		string port;
		struct sockaddr_in connect_addr;
		char sendBuffer[BUFMAX];
	public:
		TCPComm() = delete;
		TCPComm(string);
		~TCPComm() = default;
		void run(int);
		void connectSocket();
		void setDeviceIndex();
		void sendDataObject();
};

class ReplyComm{
	private:
		int sd_udp;
		int sd_tcp;
		string port_udp;
		string port_tcp;
		struct sockaddr_in s_addr_udp, s_addr_tcp, c_addr;
		struct pollfd sock_pollfd[SOCKETMAX];
		const struct pollfd* pollfd_end = &sock_pollfd[SOCKETMAX-1];
		char recvBuffer[BUFMAX], sendBuffer[BUFMAX];
	public:
		ReplyComm() = delete;
		ReplyComm(string,string);
		~ReplyComm() = default;
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
