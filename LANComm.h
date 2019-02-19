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
#include <unordered_map>
#include <sys/poll.h>
#include <algorithm>
#include <thread>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

#define BUFMAX 1024
#define DEVICEMAX 100
#define SOCKETMAX 100

using namespace std;

extern vector<sockaddr_in> deviceInfo;
extern unordered_map<string, sockaddr_in> g_IPtoDeviceTable;
extern string g_userID;

class UDPComm{
	public:
		UDPComm() = delete;
		UDPComm(string);
		~UDPComm() = default;
		void run();
		int createBrdSocket();
		int createUniSocket();
		string getBrdIp();
		void brdcast(string);
		void unicast(string);
		void recvMultipleResponse(int);
		int checkDeviceInfo(sockaddr_in);
		int checkUserID(string);
		void AlarmTimer(int);
		void setAlarmInfo();
	private:
		int sd_brdcast_;
		int sd_unicast_;
		int status_;
		string brdIp_;
		string port_;
		struct sigaction act_;
		struct sockaddr_in s_addr_;
		struct sockaddr_in c_addr_;
		//char sendBuffer_[BUFMAX];
		char recvBuffer_[BUFMAX];
		//AES aes; // aes 객체 추가해야함
};

class TCPComm{
	public:
		TCPComm() = delete;
		TCPComm(string);
		~TCPComm() = default;
		void run(int);
		void connectSocket();
		void setDeviceIndex();
		void sendDataObject();
	private:
		int sd_;
		int select_num_;
		string port_;
		struct sockaddr_in connect_addr_;
		char sendBuffer_[BUFMAX];
};

class ReplyComm{
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
	private:
		int sd_udp_;
		int sd_tcp_;
		string port_udp_;
		string port_tcp_;
		struct sockaddr_in s_addr_udp_, s_addr_tcp_, c_addr_;
		struct pollfd sock_pollfd_[SOCKETMAX];
		const struct pollfd* pollfd_end_ = &sock_pollfd_[SOCKETMAX-1];
		char recvBuffer_[BUFMAX], sendBuffer_[BUFMAX];
};


class UserActivityJson{
	public:
		UserActivityJson(string, string, string, string);
		void setUserID(string);
		void setActivityType(string);
		void setDeviceType(string);
		void setDeviceName(string);
		string getClassName();
		string getUserID();
		string getActivityType();
		string getDeviceType();
		string getDeviceName();
		template <typename Writer> void serializer(Writer&) const;
		void deserializer(const char*);
	private:
		string class_name_="UserActivityJson";
		string user_id_;
		string activity_type_;
		string device_type_;
		string device_name_;
};

class UserIDJson{
	public:
		string getClassName();
		UserIDJson(string);
		string getUserID();
		void setUserID(string);
		template <typename Writer> void serializer(Writer&) const;
		void deserializer(const char*);
	private:
		string class_name_="UserIDJson";
		string user_id_;
};

class DeviceIDJson{
	public:
		DeviceIDJson(string);
		string getClassName();
		string getDeviceID();
		void setDeviceID(string);
		template <typename Writer> void serializer(Writer&) const;
		void deserializer(const char*);
	private:
		string class_name_="DeviceIDJson";
		string device_id_;
};

class PubKeyJson{
	public:
		PubKeyJson(string);
		string getClassName();
		string getPubKey();
		void setPubKey(string);
		template <typename Writer> void serializer(Writer&) const;
		void deserializer(const char*);
	private:
		string class_name_="PubKeyJson";
		string public_key_;
};

class JsonParser {
	public:
		JsonParser();
		JsonParser(const char*); 
		void setMessage(const char*);
		bool hasMember(const char*);
		string getString(const char*);    
	private:
		rapidjson::Document doc_;
};

void sigAlarm(int);

#endif
