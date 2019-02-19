LANComm: 
	g++ -o LANComm main.cc UDPComm.cc TCPComm.cc ReplyComm.cc UserActivity.cc JsonParser.cc LANComm.h -lpthread
all: LANComm

clean:
	rm LANComm
