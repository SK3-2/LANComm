LANComm: 
	g++ -o LANComm UDPComm.cpp TCPComm.cpp ReplyDaemon.cpp main.cpp LANComm.h -lpthread
all: LANComm

clean:
	rm LANComm
