#define BACKLOG 50
#define TCP_LISTEN_PORT 50000
#define MSG_MAX_SIZE 100 // 100 byts max sent by peer
#include "common.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <iostream>
#include <list>
#include <thread>
#include <time.h>
#include <poll.h>
#include <plog/Log.h>
#include <mutex>
#include "runnable.hpp"

class TcpServer:public runnable
{
public:
	TcpServer():m_pollTimeout{1,1000000000},m_tcpPort(TCP_LISTEN_PORT)
	{
		PLOG_INFO << " Creating object";
		this -> createSocket();
		PLOG_INFO << " Socket created and set to Listen";
		//launch new thread that does manages the connections
		m_connectionMgmThread = std::make_unique<std::thread>(&TcpServer::manageConnections,this);
		//manageConnections();
		//on the current thread just process data (poll for read)
			//this -> receiveData();
		receiveData();
		m_connectionMgmThread -> join();

	}
	~TcpServer(){
		//close all the remaining connections
		std::cout << "[TCP Server Destructor] Closing all the connections alive" << std::endl;
	}	


protected:


private:

	void manageConnections(); 


	u_int16_t m_tcpPort;
	std::unique_ptr<std::thread> m_connectionMgmThread;
	std::list<int> m_connectedClientsFds;
	//std::list<sockaddr*> connectedClients;
	struct sockaddr_in m_Address;
	int sockfd;
	int createSocket();
	//get connected clients list and convert to an array of pollfd. Used for poll syscall
	void fdToPollFdArray(pollfd* pollfds);
	void receiveData(message_t* receivedMessage);
	void receiveData();
	std::string getPeerIp(sockaddr* addr);
    uint16_t getPeerPort(sockaddr* addr);
    std::mutex m_connectedClientsMutex;
	const timespec m_pollTimeout;
};