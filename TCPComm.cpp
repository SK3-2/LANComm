#include "LANComm.h"

vector<sockaddr_in> deviceInfo={};
string UserID = "taehyun";

TCPComm::TCPComm(string port_t): port(port_t) {
	memset((char *)&connect_addr,'\0',sizeof(connect_addr));
}

void TCPComm::run(int select_num_t) {
	select_num = select_num_t;
	setDeviceIndex();
	connectSocket();
	sendDataObject();
}

void TCPComm::connectSocket() {
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{ //return sd when success, or -1
		cout<<"Client : Can't open stream socket."<<endl;
		exit(1);
	}
	
	// connect 
	if((connect(sd,(struct sockaddr *)&connect_addr,sizeof(connect_addr))) == -1)
	{
		cout<<"Client : Can't connect to server."<<endl;
		close(sd);
		exit(1);
	} 

	cout<<"Connect..."<<endl;
}

void TCPComm::sendDataObject() {
	while(1) {
		int n_read;
		if((n_read = read(0, sendBuffer, BUFMAX)) > 0) {
			sendBuffer[n_read-1] = '\0';
		}
		string msg = string(sendBuffer);
		msg.append(" [from 192.168.0.103]");
		send(sd, msg.c_str(), msg.length(), 0);
	}
}

void TCPComm::setDeviceIndex() {
	connect_addr = deviceInfo[select_num];	
	cout<<"MY TCP tries to connect to "<<inet_ntoa(connect_addr.sin_addr)<<endl;
	connect_addr.sin_port = htons(atoi(port.c_str()));
}
