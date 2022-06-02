#include "proxy.hpp"
#include <stdexcept>
#include <errno.h>
#include <cstring>
#include <chrono>
#include <thread>
#include <poll.h>
#include <unistd.h>

int TcpServer::createSocket(){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

	memset(&m_Address,0,sizeof(sockaddr_in)); //init address to 0
	m_Address.sin_family = AF_INET; //set ip socket
	m_Address.sin_port = htons(m_tcpPort);
	m_Address.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(sockfd, (struct sockaddr *) &m_Address, sizeof(sockaddr_in)) < 0)
		throw std::runtime_error(std::strerror(errno));
	listen(sockfd,BACKLOG);

	return sockfd;
}

/*  Manage TCP Connections
- accept clients that want to establish connection
- store their File Descriptors in a list
- remove File descriptors from list when connections are closed
- close connections when TCP server is closed
*/
void TcpServer::manageConnections(){


	PLOG_INFO << " Manage Connection Thread";
	using namespace std::chrono_literals;
	sockaddr* clientAddr = new sockaddr();
	socklen_t socketSize = sizeof(socklen_t);
	while (true) {
		//blocking call: if no connection done we will be blocked here instead of closing other connections
		const int fd = accept(sockfd, clientAddr,&socketSize);
		if (fd < 0)
			throw std::runtime_error(std::strerror(errno));

		PLOG_INFO << " Accepted new connection with : " <<  getPeerIp(clientAddr) << " : " << getPeerPort(clientAddr);

		std::lock_guard<std::mutex> guard(m_connectedClientsMutex);
		m_connectedClientsFds.push_back(fd);
		std::this_thread::sleep_for(2000ms);
	}
}

/* Poll the list of sockets.
*/
void TcpServer::receiveData(){
	pollfd* pollfds = new pollfd[BACKLOG];
	char* msgBuf = new char[MSG_MAX_SIZE];
	int ret;
	//TODO: poll timeout could be set as a parameter of a function parsed for config
	while (true) // replace with is running or something like that
	{
		fdToPollFdArray(pollfds);
		ret = poll(pollfds,m_connectedClientsFds.size(), 1000);
		/* On success, poll() returns a nonnegative value which is the
	       number of elements in the pollfds whose revents fields have been
	       set to a nonzero value (indicating an event or an error).  A
	       return value of zero indicates that the system call timed out
	       before any file descriptors became read.
	    */
	   	PLOG_DEBUG << "Poll return " << ret;

	    if (ret != 0) {
	    	for (int i=0;i<m_connectedClientsFds.size();i++){
	    		if (pollfds[i].revents & (POLLHUP | POLLERR)){ 
	    		    PLOG_INFO << "Socket " << pollfds[i].fd << " has hung up ";
	    			std::lock_guard<std::mutex> guard(m_connectedClientsMutex);
	    			m_connectedClientsFds.remove(pollfds[i].fd);
	    		}
	    		   //call method that removes fd from list
	    		else if (pollfds[i].revents & POLLIN ) {
	    	       if (read(pollfds[i].fd, msgBuf, MSG_MAX_SIZE) != 0){
	    	       	PLOG_INFO << "From " << pollfds[i].fd << std::string(msgBuf);
	    	       }
	    	    } else {
	    	    	PLOG_INFO << "on FD " << pollfds[i].fd << "Unknown poll revent: " << pollfds[i].revents;
	    	    }
	    	}
	    } else {
	    	PLOG_INFO << "Poll Time out on " << ret << " sockets";
	    }
		sleep(2);
	}

    delete msgBuf;
    delete pollfds;
}

std::string TcpServer::getPeerIp(sockaddr* addr){
	struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
    //const char *s = 
    //*s = inet_ntoa(addr_in->sin_addr);
    //std:: cout << "Debug 3 "<< s << "\n" ;
    return std::string(inet_ntoa(addr_in->sin_addr));
}

uint16_t TcpServer::getPeerPort(sockaddr* addr){
	struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
    return addr_in->sin_port;
}


void TcpServer::fdToPollFdArray(pollfd* pollfds ){
	int pos = 0;
	PLOG_DEBUG << "Having  " << m_connectedClientsFds.size() << " FDs";

	for (auto i:m_connectedClientsFds)
	{   
		PLOG_DEBUG << "Fetching FD for client : " << i;
		pollfds[pos].fd    = i;
		pollfds[pos].events = (POLLIN | POLLPRI | POLLERR); 
		/*this will poll for the following:
		#define POLLIN                0x001                 There is data to read.  
		#define POLLPRI                0x002                 There is urgent data to read.  
		*/
		pos++;

	}
}