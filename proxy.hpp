#define BACKLOG 50
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
#include "dataManager.hpp"
#ifndef PROXY_H
#define PROXY_H

class TcpServer:public runnable
{
public:
	/*constructor uses data manager pointer that well be used to get a data manager port and 
	  use its methods for data management */
	TcpServer(uint16_t tcpPort,std::shared_ptr<dataManager<std::string>> dataHandler):m_pollTimeout{1,1000000000},m_tcpPort(tcpPort),m_dataHandler(dataHandler)
	{
		PLOG_INFO << " Creating object";
		createSocket();
	}
	~TcpServer(){
	}	

	//in running state all the magic operates
	void run();

protected:


private:

	//data handler, used when receiving packets
	std::shared_ptr<dataManager<std::string>> m_dataHandler;
	uint16_t m_tcpPort;
	std::list<int> m_connectedClientsFds;
	struct sockaddr_in m_Address;
	int sockfd;
	
	int createSocket();
	void manageConnections(); 
	//get connected clients list and convert to an array of pollfd. Used for poll syscall
	void fdToPollFdArray(pollfd* pollfds);
	void receiveData(std::shared_ptr<dataManager<std::string>> dataHandler);
	std::string getPeerIp(sockaddr* addr);
    uint16_t getPeerPort(sockaddr* addr);
    std::mutex m_connectedClientsMutex;
	const timespec m_pollTimeout;
};

#endif