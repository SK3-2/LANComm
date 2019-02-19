#include "LANComm.h"

int main(int argc, char *argv[]) {

	if(argc < 3) {
		cout<<"Usage: "<<argv[0]<<" [UDP_PORT] [TCP_PORT]"<<endl;
	}
	
	ReplyComm r1(argv[1],argv[2]);
	thread receiver([&](){r1.run();});

	UDPComm cm1(argv[1]);
	cm1.run();
	
	cout<<"TCP connect..."<<endl;

	TCPComm cm2(argv[2]);
	
	cout<<"success"<<endl;
	cout<<"Set the device number to connect"<<endl;
	
	int select_num;
	cin >> select_num;
	cm2.run(select_num);

}


