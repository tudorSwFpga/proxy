#include <memory>
#include <thread>
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include "proxy.hpp"


int main()
{
	plog::init(plog::debug, "/tmp/test.log", 1000000, 5);
	/* code */
	PLOG_INFO << "Starting System";
	std::unique_ptr<TcpServer> tcpServer = std::make_unique<TcpServer>();
	/*while (true) {
		tcpServer -> manageConnections();
	}*/
	//launch proxy
	//launch calculator
	//launch logger

	return 0;
}