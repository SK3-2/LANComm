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
    void createUDPSocket();
    int createBrdSocket();
    int createUniSocket();
    string getBrdIp();
    void brdcast(string);
    void unicast(sockaddr_in&, string);
    void recvMultipleResponse(int);
    int checkDeviceInfo(sockaddr_in);
    int checkUserID(string);
    void setRSAEncryptor(const RSA_Encryptor&);
  private:
    int sd_brdcast_;
    int sd_unicast_;
    string brdIp_;
    string port_;
    struct sockaddr_in s_addr_;
    struct sockaddr_in c_addr_;
    //char sendBuffer_[BUFMAX];
    char recvBuffer_[BUFMAX];
    const RSA_Encryptor* rsaEn_;
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



////////////////////////////////////////////////////////
class Receiver{
  public:
    //Constructor
    Receiver() = delete;
    Receiver(string,string);	  
    int createUDPSocket();
    int createTCPSocket();
    void setHeaderAnalyzer(HeaderAnalyzer*);

    //Receiving message
    void run();
    bool recvMsg_TCP(int,string&);
    bool recvMsg_UDP(string&);

    //Poll 
    int getEmptyPfdIndex();
    struct pollfd* getNextPollfd(struct pollfd*);
    struct pollfd* getNextReventPoll(struct pollfd*);
    int acceptPollfd(int);
    void register_Pollfd(int);
  private:
    //HeaderAnalyzer
    HeaderAnalyzer* headerAnalyzer;
    
    //Network
    int sd_udp_;
    int sd_tcp_;		
    string port_udp_;
    string port_tcp_;
    UDPComm* UDPComm_;
    TCPComm* TCPComm_;
    struct sockaddr_in s_addr_udp_, s_addr_tcp_, c_addr_;

    //Poll
    struct pollfd sock_pollfd_[SOCKETMAX];
    const struct pollfd* pollfd_end_ = &sock_pollfd_[SOCKETMAX-1];
    char recvBuffer_[BUFMAX];

};
///////////////////////////////////////////////////////

#endif
