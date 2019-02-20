#include "LANComm.h"

vector<sockaddr_in> deviceInfo={};
unordered_map<string, sockaddr_in> g_IPtoDeviceTable; string UserID = "taehyun";

TCPComm::TCPComm(string port): port_(port) {
	memset((char *)&connect_addr_,'\0',sizeof(connect_addr_));
}

void TCPComm::run(int select_num) {
	select_num_ = select_num;
	setDeviceIndex();
	connectSocket();
	sendDataObject();
}

void TCPComm::connectSocket() {
	if ((sd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{ //return sd when success, or -1
		cout<<"Client : Can't open stream socket."<<endl;
		exit(1);
	}
	
	// connect 
	if((connect(sd_,(struct sockaddr *)&connect_addr_,sizeof(connect_addr_))) == -1)
	{
		cout<<"Client : Can't connect to server."<<endl;
		close(sd_);
		exit(1);
	} 

	cout<<"Connect..."<<endl;
}

void TCPComm::sendDataObject() {
	while(1) {
		int n_read;
		if((n_read = read(0, sendBuffer_, BUFMAX)) > 0) {
			sendBuffer_[n_read-1] = '\0';
		}
		string msg = string(sendBuffer_);
		msg.append(" [from 192.168.0.103]");
		send(sd_, msg.c_str(), msg.length(), 0);
	}
}

void TCPComm::setDeviceIndex() {
	connect_addr_ = deviceInfo[select_num_];	
	cout<<"MY TCP tries to connect to "<<inet_ntoa(connect_addr_.sin_addr)<<endl;
	connect_addr_.sin_port = htons(atoi(port_.c_str()));
}
