#include "LANComm.h"

vector<sockaddr_in> deviceInfo={};

TCPComm::TCPComm() {
	memset((char *)&connect_addr,'\0',sizeof(connect_addr));
//	connect_addr.sin_family = AF_INET;
//	connect_addr.sin_port = htons(atoi(port));
//	connect_addr.sin_addr.s_addr =  inet_addr(ip);
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

void TCPComm::run() {
	connectSocket();
	string msg="hi, I'm TCP ";
	//string msg =("hi, I'm TCP ").append("from "+string(inet_ntoa(connect_addr.sin_addr));
	msg.append("from ").append("192.168.0.103\n");
	send(sd, msg.c_str(), msg.length(), 0);
}

void TCPComm::setDeviceIndex(int index) {
	connect_addr = deviceInfo[index];	
	cout<<"MY TCP tries to connect to "<<inet_ntoa(connect_addr.sin_addr)<<endl;
	string port = "77778";
	connect_addr.sin_port = htons(atoi(port.c_str()));
}
