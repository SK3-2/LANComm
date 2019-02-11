#include "LANComm.h"

//UDPComm::UDPComm() {}

//vector<sockaddr_in> deviceInfo;

UDPComm::UDPComm(string brdIp, string port): brdIp(brdIp), port_udp(port) {
	//alarm signal settting
	act.sa_handler = sigAlarm;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	status = sigaction(SIGALRM, &act, 0);
}

void UDPComm::run() {
	createBrdSocket();
	setLocalDeviceInfo();
}

int UDPComm::createBrdSocket() {

	// broadcast ip & port setting
	bzero(&s_addr_s, sizeof(s_addr_s));
	s_addr_s.sin_family = AF_INET;
	s_addr_s.sin_addr.s_addr = inet_addr(brdIp.c_str());
	s_addr_s.sin_port = htons(atoi(port_udp.c_str()));

	//UDP comm setting
	int sd = socket(AF_INET, SOCK_DGRAM, 0);

	if((status = setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on))) !=0) {
		cout<<"setsockopt error\n"<<endl;
		exit(-1);
	}

	return sd;
}

void UDPComm::setLocalDeviceInfo() {
	int n_read, n_send;
	int sd = createBrdSocket();

	if((n_read = read(0, sendBuffer, BUFSIZ)) > 0) {
		sendBuffer[n_read-1] = '\0';
	}

	if((n_send = sendto(sd, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr *)&s_addr_s, sizeof(s_addr_s))) < 0) {
		cout<<"sendto() error"<<endl;
		exit(-3);
	}

	recvMultipleResponse(sd);
	close(sd);
}

void UDPComm::recvMultipleResponse(int sd) {

	int addr_len = sizeof(c_addr);
	int n_recv, ret;
	while(1) {
		AlarmTimer(2);
		if((n_recv = recvfrom(sd, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr *)&c_addr, (socklen_t *)&addr_len))<0) {

			if(errno == EINTR) {
				cout<<"socket timeout"<<endl;
				cout<<"success"<<endl;
				break;
			}
			else {
				cout<<"recvfrom error"<<endl;
			}
		} 
		else {
			AlarmTimer(0);
			recvBuffer[n_recv]='\0';
			cout<<"echoed Data: "<<recvBuffer<<" from "<<inet_ntoa(c_addr.sin_addr)<<endl;

			if((ret = checkDeviceInfo(c_addr))==0) {
				continue;
			}
			else {
				//LocalDevice에 나중에 생길 여러 정보를 담아서 객체 생성 후 vector에 저장
				deviceInfo.push_back(c_addr);
				cout<<"save ip["<<inet_ntoa(c_addr.sin_addr)<<"]"<<endl;
			}
		}
	}

}

int UDPComm::checkDeviceInfo(sockaddr_in checkaddr_in) {

	cout<<"check!"<<endl;
	for(auto it = deviceInfo.begin(); it!=deviceInfo.end(); ++it) {
		if (((*it).sin_addr.s_addr == checkaddr_in.sin_addr.s_addr) & ((*it).sin_port == checkaddr_in.sin_port)) {
			return 0;
		}
	}
	return 1;
}

void UDPComm::AlarmTimer(int time) {
	alarm(time);
	return;
}

void sigAlarm(int signo) {
	cout<<"alarm\n"<<endl;
	return;
}


/*
int main(int argc, char* argv[]) {
	int sd, sd_listen;
	int c_socket;
	struct sockaddr_in s_addr, s_addr_listen, c_addr;
	char sendBuffer[BUFSIZ], recvBuffer[BUFSIZ];
	int n, n_send, n_recv, status;
	int addr_len;

	int on = 1;

	if(argc !=3) {
		cout<<"usage: UDPbcaster broadcast_ip_addr port\n"<<endl;
		exit(-1);
	}

	sd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = inet_addr(argv[1]);
	s_addr.sin_port = htons(atoi(argv[2]));

	// tcp/ip 접속을 위한 socket
	sd_listen = socket(PF_INET, SOCK_STREAM, 0);

	bzero(&s_addr_listen, sizeof(s_addr_listen));
	s_addr_listen.sin_family = AF_INET;
	s_addr_listen.sin_addr.s_addr = INADDR_ANY;
	s_addr_listen.sin_port = htons(atoi(argv[2]));

	//소켓번호는 응용 프로그램이 알고 있는 통신 창구 번호이고, 소켓주소는 네트워크 시스템(TCP/IP, UDP)이 알고있는 주소이므로 이들의 관계를 묶어두어야(bind) 응용 프로세스와 네트워크 시스템간의 패킷 전달이 가능하기 때문
	if(bind(sd_listen, (struct sockaddr *)&s_addr_listen, sizeof(s_addr_listen)) !=0) {
		cout<<"bind error\n"<<endl;
		exit(-1);
	}

	if(listen(sd_listen, 5) == -1) {
		cout<<"listen Fail\n"<<endl;
		exit(-1);
	}

	if((status = setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on))) !=0) {
		cout<<"setsockopt error\n"<<endl;;
		exit(-1);
	}

	while(1) {
		fprintf(stderr, "Type broadcasting data : ");

		if((n = read(0, sendBuffer, BUFSIZ)) > 0) {
			sendBuffer[n-1] = '\0';

			if((n_send = sendto(sd, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr *)&s_addr, sizeof(s_addr))) < 0) {
				cout<<"sendto() error"<<endl;
				exit(-3);
			}

			//			UDP response 
			addr_len = sizeof(c_addr);
			if((n_recv = recvfrom(sd_listen, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr *)&c_addr, (socklen_t *)&addr_len))<0) {
				cout<<"recvfrom error"<<endl;
				exit(-4);
			}
			recvBuffer[n_recv]='\0';
			cout<<n_recv<<endl;
			cout<<recvBuffer<<endl;

			//TCP/IP response

			addr_len = sizeof(c_addr);
			c_socket = accept(sd_listen, (struct sockaddr *) &c_addr, (socklen_t*)&addr_len);

			if((n = read(c_socket, recvBuffer, sizeof(recvBuffer))) < 0) {
				exit(-1);
			}

			recvBuffer[n] = '\0';
			cout<<"received Data: "<<recvBuffer<<"\n"<<endl;


			recvBuffer[n_recv]='\0';
			cout<<n_recv<<endl;
			cout<<recvBuffer<<endl;

			if (!strncmp(sendBuffer, "quit", 4)) {
				break;
			}
		}
	}

	close(sd);
	close(sd_listen);

	return 0;
}
*/
