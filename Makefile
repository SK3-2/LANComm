func: 
	g++ -o main_func UDPComm.cpp TCPComm.cpp ReplyDaemon.cpp main.cpp LANComm.h -lpthread
daemon:
	g++ -o main_daemon UDPComm.cpp TCPComm.cpp ReplyDaemon.cpp main_client.cpp LANComm.h
all: func daemon

clean:
	rm main_func main_daemon
