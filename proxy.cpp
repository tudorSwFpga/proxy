#include <stdexcept>
#include <errno.h>
#include <cstring>
#include <chrono>
#include <poll.h>
#include <unistd.h>
#include <plog/Log.h>


template<class T>
int TcpServer<T>::createSocket(){
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
	PLOG_INFO << " Socket created; Listening on port: " << m_tcpPort;


	return sockfd;
}

template<class T>
void TcpServer<T>::run(){
	while(m_isRunning){
		manageConnections();
		receiveData(m_dataHandler);	
	}
	
}

/*  Manage TCP Connections
- accept clients that want to establish connection
- store their File Descriptors in a list
- remove File descriptors from list when connections are closed
- close connections when TCP server is closed
*/
template<class T>
void TcpServer<T>::manageConnections(){


	PLOG_INFO << " Manage Connections";
	using namespace std::chrono_literals;
	sockaddr* clientAddr = new sockaddr();
	socklen_t socketSize = sizeof(socklen_t);
	int flags = fcntl(sockfd,F_GETFL,0);
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
	const int fd = accept(sockfd, clientAddr,&socketSize);
	if (fd < 0)
		PLOG_DEBUG << " No client connection request pending";
	else{
		PLOG_INFO << " Accepted new connection with : " <<  getPeerIp(clientAddr) << " : " << getPeerPort(clientAddr);
		std::lock_guard<std::mutex> guard(m_connectedClientsMutex);
		m_connectedClientsFds.push_back(fd);
	}
}

/* Poll the list of sockets.
*/
template<class T>
void TcpServer<T>::receiveData(std::shared_ptr<dataManager<T>> dataHandler){
	PLOG_DEBUG << "Receive data ";

	pollfd* pollfds = new pollfd[BACKLOG];
	char* msgBuf = new char[MSG_MAX_SIZE];
	int ret;
	//TODO: poll timeout could be set as a parameter of a function parsed for config
	//while (true) // replace with is running or something like that
	//{
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
	    	       	dataHandler->push(std::string(msgBuf),"TCPServer");
	    	       }
	    	    } else {
	    	    	PLOG_INFO << "on FD " << pollfds[i].fd << "Unknown poll revent: " << pollfds[i].revents;
	    	    }
	    	}
	    } else {
	    	PLOG_INFO << "Poll Time out on " << ret << " sockets";
	    }
    delete[] msgBuf;
    delete[] pollfds;
}

template<class T>
std::string TcpServer<T>::getPeerIp(sockaddr* addr){
	struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
    return std::string(inet_ntoa(addr_in->sin_addr));
}

template<class T>
uint16_t TcpServer<T>::getPeerPort(sockaddr* addr){
	struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
    return addr_in->sin_port;
}


template<class T>
void TcpServer<T>::fdToPollFdArray(pollfd* pollfds ){
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