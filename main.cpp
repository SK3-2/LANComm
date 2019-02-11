#include "LANComm.h"

int main(int argc, char *argv[]) {

	if(argc < 3) {
		cout<<"Usage: "<<argv[0]<<" [BROADCAST_ADDRESS] [PORT]"<<endl;
	}

	UDPComm cm1(argv[1],argv[2]);
	cm1.run();

	cout<<"TCP connect..."<<endl;
	TCPComm cm2;
	cm2.setDeviceIndex(1);
	cm2.run();

}
