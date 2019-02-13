#include "LANComm.h"

UDPComm::UDPComm(string port_t): port(port_t) {
	//get enable brd-ip
	string brdIp = getBrdIp();
	cout<<"my brdIP: "<<brdIp<<endl;
}

void UDPComm::setAlarmInfo() {
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
	int status;
	int on=1;
	bzero(&s_addr, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = inet_addr(brdIp.c_str());
	s_addr.sin_port = htons(atoi(port.c_str()));

	//UDP comm setting
	int sd = socket(AF_INET, SOCK_DGRAM, 0);

	if((status = setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on))) !=0) {
		cout<<"setsockopt error\n"<<endl;
		exit(-1);
	}

	return sd;
}

string UDPComm::getBrdIp() {
	char  buff[BUFMAX];
	char  cmd[BUFMAX];
	FILE *fp, *fp2;

	if ((fp = popen("ip a | awk '/state UP/ {print $2}'", "r")) == NULL) {   
		perror( "popen() 실패");
		exit(-1);
	}

	fscanf(fp, "%s", buff);
	buff[strlen(buff)-1]='\0';
	sprintf(cmd, "ip a s dev %s | awk '/inet/ {print $4}'", buff);

	if ((fp2 = popen(cmd, "r")) == NULL) {
		perror( "popen() 실패");
		exit(-1);
	}   

	fscanf(fp2, "%s", buff);
	pclose(fp);

	return string(buff);
}

void UDPComm::setLocalDeviceInfo() {
	int n_read, n_send;
	int sd = createBrdSocket();

	if((n_read = read(0, sendBuffer, BUFSIZ)) > 0) {
		sendBuffer[n_read-1] = '\0';
	}

	if((n_send = sendto(sd, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr *)&s_addr, sizeof(s_addr))) < 0) {
		cout<<"sendto() error"<<endl;
		exit(-3);
	}

	recvMultipleResponse(sd);
	close(sd);
}

void UDPComm::recvMultipleResponse(int sd) {
	int addr_len = sizeof(c_addr);
	int n_recv, ret;
	//set Alarm Signal Info
	setAlarmInfo();

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

			if((ret = checkDeviceInfo(c_addr))==0) {
				continue;
			}
			else {
				//ip 정보를 담아서 객체 생성 후 vector에 저장
				deviceInfo.push_back(c_addr);
				cout<<"echoed Data: "<<recvBuffer<<" from "<<inet_ntoa(c_addr.sin_addr)<<endl;
				cout<<"save ip["<<inet_ntoa(c_addr.sin_addr)<<"]"<<endl;
			}
		}
	}
}

int UDPComm::checkUserID(string ID) {

}

int UDPComm::checkDeviceInfo(sockaddr_in checkaddr_in) {
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
	cout<<"alarm"<<endl;
	return;
}


