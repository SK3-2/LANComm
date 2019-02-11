#include "LANComm.h"

int main(int argc, char *argv[]) {

	if(argc < 4) {
		cout<<"Usage: "<<argv[0]<<" [BROADCAST_ADDRESS] [UDP_PORT] [TCP_PORT]"<<endl;
	}
	
	ReplyDaemon r1(argv[2]);
	thread receiver([&](){r1.run();});

	UDPComm cm1(argv[1],argv[2]);
	cm1.run();
	
	cout<<"TCP connect..."<<endl;

	TCPComm cm2(argv[3]);
	cm2.setDeviceIndex(1);
	cm2.run();

}
