#include <memory>
#include <thread>
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include "proxy.hpp"
#include "dataManager.hpp"

int main()
{
	plog::init(plog::debug, "/tmp/test.log", 1000000, 5);
	/* code */
	PLOG_INFO << "Starting System";
	//create data manager
	std::shared_ptr<dataManager<std::string>> dataM = std::make_shared<dataManager<std::string>>(1,2);
	//set interfaces for the data manager
	dataM->setFeeder("TCPServer");
	dataM->setConsumer("FlightCalculator");
	dataM->setConsumer("CollisionCalculator");
	//create tcp server 
	std::unique_ptr<TcpServer> tcpServer = std::make_unique<TcpServer>(dataM);

	/*while (true) {
		tcpServer -> manageConnections();
	}*/
	//launch proxy
	//launch calculator
	//launch logger

	return 0;
}