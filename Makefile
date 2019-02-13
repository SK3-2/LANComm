LANComm: 
	g++ -std=c++11 -o LANComm main.cpp UDPComm.cpp TCPComm.cpp ReplyComm.cpp LANComm.h -lpthread
all: LANComm

clean:
	rm LANComm
