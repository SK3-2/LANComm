#include "Continuity_Channel.h"

/*///////////////////////////////////////////////////////////////////////
* Constructor & Class Referencing
*///////////////////////////////////////////////////////////////////////
Receiver::Receiver(string port_udp, string port_tcp): port_udp_(port_udp), port_tcp_(port_tcp)
{
  for(int i=0; i<SOCKETMAX; i++){
    sock_pollfd_[i] = {-1,POLLIN, 0};
  }
  sock_pollfd_[0].fd = createUDPSocket();
  sock_pollfd_[1].fd = createTCPSocket();
}

void Receiver::setHeaderAnalyzer(HeaderAnalyzer* hA){
  headerAnalyzer = hA; 
}

int Receiver::createUDPSocket() {
  // UDP comm setting
  sd_udp_ = socket(AF_INET, SOCK_DGRAM, 0);
  // broadcast ip & port setting
  bzero(&s_addr_udp_, sizeof(s_addr_udp_));
  s_addr_udp_.sin_family = AF_INET;
  s_addr_udp_.sin_addr.s_addr = INADDR_ANY;
  s_addr_udp_.sin_port = htons(atoi(port_udp_.c_str()));
  cout<<"UDP PORT: "<<atoi(port_udp_.c_str())<<endl;

  if(bind(sd_udp_, (struct sockaddr *)&s_addr_udp_, sizeof(s_addr_udp_))) {
    cout<<"UDP Bind Error."<<endl;
    exit(-2);
  }

  return sd_udp_;
}

int Receiver::createTCPSocket() {
  // tcp/ip 접속을 위한 socket
  sd_tcp_ = socket(PF_INET, SOCK_STREAM, 0);
  bzero(&s_addr_tcp_, sizeof(s_addr_tcp_));
  s_addr_tcp_.sin_family = AF_INET;
  s_addr_tcp_.sin_addr.s_addr = INADDR_ANY;
  s_addr_tcp_.sin_port = htons(atoi(port_tcp_.c_str()));
  cout<<"TCP PORT: "<<atoi(port_tcp_.c_str())<<endl;

  if(bind(sd_tcp_, (struct sockaddr *)&s_addr_tcp_, sizeof(s_addr_tcp_))) {
    cout<<"TCP Bind Error"<<endl;
    exit(-1);
  }

  if(listen(sd_tcp_, 5) == -1) {
    cout<<"TCP Listen Error"<<endl;
    exit(-1);
  }
  return sd_tcp_;
}

   
/*///////////////////////////////////////////////////////////////////////
* Poll loop
*///////////////////////////////////////////////////////////////////////
void Receiver::run() {
  cout<<"Receiver run!"<<endl;
  while(1){
    int nread = poll(sock_pollfd_, SOCKETMAX, -1);
    struct pollfd* cur = NULL;    

    for (int n_poll = 0; n_poll < nread; n_poll++) {  // iterate nread times
      cur = getNextReventPoll(cur);  // return type : struct pollfd*
      int index = cur-&sock_pollfd_[0];

      if(index == 0) {
	cout<<"UDP Socket Event Ocuur!"<<endl;
	string buffer;
	recvMsg_UDP(buffer);	
	//HeaderAnalyzer();
      }
      else if(index == 1) {
	cout<<"TCP/IP Socket Event Occur!"<<endl;
	int nfd = acceptPollfd(sd_tcp_);	
	register_Pollfd(nfd);	
      }
      else if(index > 1){
	cout<<"Client Socket Event Occur!"<<endl;
	int csd = sock_pollfd_[index].fd;
	string buffer;
	recvMsg_TCP(csd,buffer);
	//HeaderAnalyzer();
      }
      else{
	cout<<"Wrong Index Input: "<<cur->fd<<endl;      
      }
    }
  }
}

/*///////////////////////////////////////////////////////////////////////
* Functions for Receiving Message   
*///////////////////////////////////////////////////////////////////////
bool Receiver::recvMsg_TCP(int fd, string& buf) {
  strcpy(recvBuffer_,""); //buf 초기화
  int ret = recv(fd, recvBuffer_, sizeof(recvBuffer_), 0);
  if (ret == 0){  // 읽어 왔으나 아무것도 없음 --> EOF 수신 
    cout<<"Client Socket Disconnected."<<endl;
    close(fd);    
    return true;
  }
  else if(ret < 0){
    cout<<"TCP Receive Error."<<endl;
    return false;
  }
  buf = string(recvBuffer_);
  return true;
  //cout<<"tcp recv: "<<sendBuffer_<<endl;
}


bool Receiver::recvMsg_UDP(string& buf) {
  int addr_len = sizeof(c_addr_);
  int n_read;
  if((n_read = recvfrom(sd_udp_, recvBuffer_, sizeof(recvBuffer_), 0, 
			(struct sockaddr *)&c_addr_, (socklen_t *)&addr_len)) < 0)
  {
    cout<<"UDP Receive Error."<<endl;
    return false;
  }
  buf = string(recvBuffer_);
  cout<<"Broadcasted from "<<inet_ntoa(c_addr_.sin_addr)<<"["<<buf<<"]"<<endl;  
  return true;
}


/* ///////////////////////////////////////////////////////////////////////
 * Functions for pollfd processing  
*/ //////////////////////////////////////////////////////////////////////

// Get Empty pollfd index
int Receiver::getEmptyPfdIndex(void){
  for (struct pollfd* pfd = &sock_pollfd_[0]; pfd < pollfd_end_; pfd++){
    if(pfd->fd == -1){
      return (int)(pfd - &sock_pollfd_[0]);
    }
  }
  return -1;
}

// Get Next assigned pollfd Ptr
struct pollfd* Receiver::getNextPollfd(struct pollfd* cur){
  int index;
  if(cur==NULL) index = 0;
  else{
    index = cur - &sock_pollfd_[0];
    index++;
  }
  //Search pfd which has fd not equal to -1
  struct pollfd* pfd;
  for(pfd = &sock_pollfd_[index]; pfd < pollfd_end_;++pfd){
    if(pfd->fd != -1) return pfd;
  }
  return NULL;
}

// Poll revent handler
struct pollfd* Receiver::getNextReventPoll(struct pollfd* pfd){
  struct pollfd* cur = pfd;
  while((cur = getNextPollfd(cur))){
    if(cur->revents == POLLIN){
      return cur;
    }				
  }
  return NULL;
}  
// Accept pollfd
int Receiver::acceptPollfd(int sd){
  struct sockaddr_in ADDR_IN;
  int serverlen = sizeof(sockaddr_in);
  int nfd = accept(sd, (struct sockaddr *)&ADDR_IN, (socklen_t *)&serverlen);
  return nfd;
}
// Register pollfd
void Receiver::register_Pollfd(int nfd) {
  int EmptyPfdIndex = getEmptyPfdIndex();
  sock_pollfd_[EmptyPfdIndex].fd = nfd;
}
