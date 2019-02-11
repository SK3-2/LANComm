#include "LANComm.h"

int main(int argc, char* argv[]) {
	
	if(argc < 2) {
		cout<<"Usage: "<<argv[0]<<" [PORT]"<<endl;
	}

	ReplyDaemon r1(argv[1]);
	r1.run();

}
