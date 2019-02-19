#include "LANComm.h"

UDPComm::UDPComm(string port): port_(port) {
	//get enable brd-ip
	string brdIp_ = getBrdIp();
	cout<<"my brdIP: "<<brdIp_<<endl;
}

void UDPComm::setAlarmInfo() {
	act_.sa_handler = sigAlarm;
	sigemptyset(&act_.sa_mask);
	act_.sa_flags = 0;
	status_ = sigaction(SIGALRM, &act_, 0);
}

void UDPComm::run() {
	sd_brdcast_ = createBrdSocket();
	sd_unicast_ = createUniSocket();
}

int UDPComm::createBrdSocket() {
	// broadcast ip & port setting
	int status;
	int on=1;
	bzero(&s_addr_, sizeof(s_addr_));
	s_addr_.sin_family = AF_INET;
	s_addr_.sin_addr.s_addr = inet_addr(brdIp_.c_str());
	s_addr_.sin_port = htons(atoi(port_.c_str()));

	//UDP comm setting
	int sd = socket(AF_INET, SOCK_DGRAM, 0);

	if((status = setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on))) !=0) {
		cout<<"setsockopt error\n"<<endl;
		exit(-1);
	}

	return sd;
}

int UDPComm::createUniSocket() {
	//UDP comm setting
	sd_unicast_ = socket(AF_INET, SOCK_DGRAM, 0);

	cout<<"UDP PORT: "<<atoi(port_.c_str())<<endl;

	return sd_unicast_;
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

void UDPComm::brdcast(string Message) {
	int n_send;
	/* test용 (terminal)
	if((n_read = read(0, sendBuffer_, BUFSIZ)) > 0) {
		sendBuffer_[n_read-1] = '\0';
	}
	*/
	if((n_send = sendto(sd_brdcast_, Message.c_str(), Message.length(), 0, (struct sockaddr *)&s_addr_, sizeof(s_addr_))) < 0) {
		cout<<"sendto() error"<<endl;
		exit(-3);
	}

	/*
	if((n_send = sendto(sd_brdcast_, sendBuffer_, strlen(sendBuffer_), 0, (struct sockaddr *)&s_addr_, sizeof(s_addr_))) < 0) {
		cout<<"sendto() error"<<endl;
		exit(-3);
	}
	*/
}

void UDPComm::unicast(sockaddr_in& c_addr, string Message) {
	int n_send;

	/* test용 (terminal)
	if((n_read = read(0, sendBuffer_, BUFSIZ)) > 0) {
		sendBuffer_[n_read-1] = '\0';
	}
	*/
	if((n_send = sendto(sd_unicast_, Message.c_str(), Message.length(), 0, (struct sockaddr *)&c_addr_, sizeof(s_addr_))) < 0) {
		cout<<"sendto() error"<<endl;
		exit(-3);
	}
	/*	
	if((n_send = sendto(sd_unicast_, sendBuffer_, strlen(sendBuffer_), 0, (struct sockaddr *)&c_addr_, sizeof(s_addr_))) < 0) {
		cout<<"sendto() error"<<endl;
		exit(-3);
	}
	*/
}

int UDPComm::checkUserID(string ID) {
	return 0;
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

