#include "LANComm.h"

ReplyComm::ReplyComm(string port_udp, string port_tcp): port_udp_(port_udp), port_tcp_(port_tcp){
	for(int i=0; i<SOCKETMAX; i++){
		sock_pollfd_[i] = {-1,POLLIN, 0};
	}

	sock_pollfd_[0].fd = createTCPSocket();
	sock_pollfd_[1].fd = createUDPSocket();
}


int ReplyComm::createUDPSocket() {
	//UDP comm setting
	sd_udp_ = socket(AF_INET, SOCK_DGRAM, 0);
	// broadcast ip & port setting
	bzero(&s_addr_udp_, sizeof(s_addr_udp_));
	s_addr_udp_.sin_family = AF_INET;
	s_addr_udp_.sin_addr.s_addr = INADDR_ANY;
	s_addr_udp_.sin_port = htons(atoi(port_udp_.c_str()));

	cout<<"UDP PORT: "<<atoi(port_udp_.c_str())<<endl;

	if(bind(sd_udp_, (struct sockaddr *)&s_addr_udp_, sizeof(s_addr_udp_)) !=0) {
		cout<<"udp bind error\n"<<endl;
		exit(-2);
	}

	return sd_udp_;
}

int ReplyComm::createTCPSocket() {
	// tcp/ip 접속을 위한 socket
	sd_tcp_ = socket(PF_INET, SOCK_STREAM, 0);
	bzero(&s_addr_tcp_, sizeof(s_addr_tcp_));
	s_addr_tcp_.sin_family = AF_INET;
	s_addr_tcp_.sin_addr.s_addr = INADDR_ANY;
	s_addr_tcp_.sin_port = htons(atoi(port_tcp_.c_str()));

	cout<<"TCP PORT: "<<atoi(port_tcp_.c_str())<<endl;

	if(bind(sd_tcp_, (struct sockaddr *)&s_addr_tcp_, sizeof(s_addr_tcp_)) !=0) {
		cout<<"tcp bind error\n"<<endl;
		exit(-1);
	}

	if(listen(sd_tcp_, 5) == -1) {
		cout<<"listen Fail\n"<<endl;
		exit(-1);
	}

	return sd_tcp_;
}
// poll udp & tcp socket
void ReplyComm::run() {
	cout<<"ReplyComm run!"<<endl;
	while(1){
		int nread = poll(sock_pollfd_, SOCKETMAX, -1);
		struct pollfd* cur = NULL;    

		for (int n_poll = 0; n_poll < nread; n_poll++) {  // iterate nread times
			cur = getNextReventPoll(cur);  // return type : struct pollfd*
			int index = cur-&sock_pollfd_[0];

			if(index == 0) {
				cout<<"TCP/IP Socket Event Occur!"<<endl;
				int nfd = acceptPollfd(sd_tcp_);	
				register_Pollfd(nfd);
				recvMsg(nfd);
			}
			else if(index == 1) {
				cout<<"UDP Socket Event Ocuur!"<<endl;
				replyDeviceInfo();	
			}
			else if(index > 1){
				cout<<"Client Socket Event Occur!"<<endl;
				int csd = sock_pollfd_[index].fd;
				recvMsg(csd);
			}
			else{
				cout<<"Wrong Index Input: "<<cur->fd<<endl;      
			}
		}
	}
}

string ReplyComm::recvMsg(int fd) {
	strcpy(sendBuffer_,""); //buf 초기화
	int ret = recv(fd, sendBuffer_, sizeof(sendBuffer_), 0);

	if (ret == 0){  // 읽어 왔으나 아무것도 없음 --> EOF 수신 
		perror("tcp is disconnected");
		close(sd_tcp_);
		exit(1);
	}

	cout<<"tcp recv: "<<sendBuffer_<<endl;
	
	return sendBuffer_;
}

void ReplyComm::replyDeviceInfo() {
	int addr_len = sizeof(c_addr_);
	int n_read, n_send;

	if((n_read = recvfrom(sd_udp_, recvBuffer_, sizeof(recvBuffer_), 0, (struct sockaddr *)&c_addr_, (socklen_t *)&addr_len)) < 0) {
		cout<<"recvfrom error"<<endl;
		exit(-3);
	}
	recvBuffer_[n_read] = '\0';

	if (!strncmp(recvBuffer_, "quit", 4)) {
		return;
	}

	string str;
	str.append("Broadcasted from ").append(inet_ntoa(c_addr_.sin_addr)).append("[").append(string(recvBuffer_)).append("]");
	cout<<str<<endl;

	if((n_send = sendto(sd_udp_, recvBuffer_, n_read, 0, (struct sockaddr *)&c_addr_, sizeof(c_addr_)))<0) {
		cout<<"sendto() error"<<endl;
		exit(-4);
	}
}

// Get Empty pollfd index
int ReplyComm::getEmptyPfdIndex(void){
	for (struct pollfd* pfd = &sock_pollfd_[0]; pfd < pollfd_end_; pfd++){
		if(pfd->fd == -1){
			return (int)(pfd - &sock_pollfd_[0]);
		}
	}
	return -1;
}

// Get Next assigned pollfd Ptr
struct pollfd* ReplyComm::getNextPollfd(struct pollfd* cur){
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
struct pollfd* ReplyComm::getNextReventPoll(struct pollfd* pfd){
	struct pollfd* cur = pfd;
	while((cur = getNextPollfd(cur))){
		if(cur->revents == POLLIN){
			return cur;
		}				
	}
	return NULL;
}  

// Accept pollfd
int ReplyComm::acceptPollfd(int sd){
	struct sockaddr_in ADDR_IN;
	int serverlen = sizeof(sockaddr_in);
	int nfd = accept(sd, (struct sockaddr *)&ADDR_IN, (socklen_t *)&serverlen);
	return nfd;
}

void ReplyComm::register_Pollfd(int nfd) {
	int EmptyPfdIndex = getEmptyPfdIndex();
	sock_pollfd_[EmptyPfdIndex].fd = nfd;
}

