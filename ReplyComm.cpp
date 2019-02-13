#include "LANComm.h"

ReplyComm::ReplyComm(string port_udp_t, string port_tcp_t): port_udp(port_udp_t), port_tcp(port_tcp_t){
	for(int i=0; i<SOCKETMAX; i++){
		sock_pollfd[i] = {-1,POLLIN, 0};
	}

	sock_pollfd[0].fd = createTCPSocket();
	sock_pollfd[1].fd = createUDPSocket();
}


int ReplyComm::createUDPSocket() {
	//UDP comm setting
	sd_udp = socket(AF_INET, SOCK_DGRAM, 0);
	// broadcast ip & port setting
	bzero(&s_addr_udp, sizeof(s_addr_udp));
	s_addr_udp.sin_family = AF_INET;
	s_addr_udp.sin_addr.s_addr = INADDR_ANY;
	s_addr_udp.sin_port = htons(atoi(port_udp.c_str()));

	cout<<"UDP PORT: "<<atoi(port_udp.c_str())<<endl;

	if(bind(sd_udp, (struct sockaddr *)&s_addr_udp, sizeof(s_addr_udp)) !=0) {
		cout<<"udp bind error\n"<<endl;
		exit(-2);
	}

	return sd_udp;
}

int ReplyComm::createTCPSocket() {
	// tcp/ip 접속을 위한 socket
	sd_tcp = socket(PF_INET, SOCK_STREAM, 0);
	bzero(&s_addr_tcp, sizeof(s_addr_tcp));
	s_addr_tcp.sin_family = AF_INET;
	s_addr_tcp.sin_addr.s_addr = INADDR_ANY;
	s_addr_tcp.sin_port = htons(atoi(port_tcp.c_str()));

	cout<<"TCP PORT: "<<atoi(port_tcp.c_str())<<endl;

	if(bind(sd_tcp, (struct sockaddr *)&s_addr_tcp, sizeof(s_addr_tcp)) !=0) {
		cout<<"tcp bind error\n"<<endl;
		exit(-1);
	}

	if(listen(sd_tcp, 5) == -1) {
		cout<<"listen Fail\n"<<endl;
		exit(-1);
	}

	return sd_tcp;
}
// poll udp & tcp socket
void ReplyComm::run() {
	cout<<"ReplyComm run!"<<endl;
	while(1){
		int nread = poll(sock_pollfd, SOCKETMAX, -1);
		struct pollfd* cur = NULL;    

		for (int n_poll = 0; n_poll < nread; n_poll++) {  // iterate nread times
			cur = getNextReventPoll(cur);  // return type : struct pollfd*
			int index = cur-&sock_pollfd[0];

			if(index == 0) {
				cout<<"TCP/IP Socket Event Occur!"<<endl;
				int nfd = acceptPollfd(sd_tcp);	
				register_Pollfd(nfd);
				recvMsg(nfd);
			}
			else if(index == 1) {
				cout<<"UDP Socket Event Ocuur!"<<endl;
				replyDeviceInfo();	
			}
			else if(index > 1){
				cout<<"Client Socket Event Occur!"<<endl;
				int csd = sock_pollfd[index].fd;
				recvMsg(csd);
			}
			else{
				cout<<"Wrong Index Input: "<<cur->fd<<endl;      
			}
		}
	}
}

string ReplyComm::recvMsg(int fd) {
	strcpy(sendBuffer,""); //buf 초기화
	int ret = recv(fd, sendBuffer, sizeof(sendBuffer), 0);

	if (ret == 0){  // 읽어 왔으나 아무것도 없음 --> EOF 수신 
		perror("tcp is disconnected");
		close(sd_tcp);
		exit(1);
	}

	cout<<"tcp recv: "<<sendBuffer<<endl;
	
	return sendBuffer;
}

void ReplyComm::replyDeviceInfo() {
	int addr_len = sizeof(c_addr);
	int n_read, n_send;

	if((n_read = recvfrom(sd_udp, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr *)&c_addr, (socklen_t *)&addr_len)) < 0) {
		cout<<"recvfrom error"<<endl;
		exit(-3);
	}
	recvBuffer[n_read] = '\0';

	if (!strncmp(recvBuffer, "quit", 4)) {
		return;
	}

	string str;
	str.append("Broadcasted from ").append(inet_ntoa(c_addr.sin_addr)).append("[").append(string(recvBuffer)).append("]");
	cout<<str<<endl;

	if((n_send = sendto(sd_udp, recvBuffer, n_read, 0, (struct sockaddr *)&c_addr, sizeof(c_addr)))<0) {
		cout<<"sendto() error"<<endl;
		exit(-4);
	}
}

// Get Empty pollfd index
int ReplyComm::getEmptyPfdIndex(void){
	for (struct pollfd* pfd = &sock_pollfd[0]; pfd < pollfd_end; pfd++){
		if(pfd->fd == -1){
			return (int)(pfd - &sock_pollfd[0]);
		}
	}
	return -1;
}

// Get Next assigned pollfd Ptr
struct pollfd* ReplyComm::getNextPollfd(struct pollfd* cur){
	int index;
	if(cur==NULL) index = 0;
	else{
		index = cur - &sock_pollfd[0];
		index++;
	}
	//Search pfd which has fd not equal to -1
	struct pollfd* pfd;
	for(pfd = &sock_pollfd[index]; pfd < pollfd_end;++pfd){
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
	sock_pollfd[EmptyPfdIndex].fd = nfd;
}

