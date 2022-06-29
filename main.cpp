#define TCP_LISTEN_PORT 50000

#include <memory>
#include <thread>
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include "proxy.hpp"
#include "dataManager.hpp"
#include "calculator.hpp"

int main()
{
	plog::init(plog::debug, "/tmp/test.log", 1000000, 5);
	PLOG_INFO << "Starting System";
	//create data manager
	std::shared_ptr<dataManager<std::string>> dataM = std::make_shared<dataManager<std::string>>(1,2);
	PLOG_INFO << "Setting data manager interfaces";
	dataM->setFeeder("TCPServer");
	dataM->setConsumer("FlightCalculator");
	dataM->setConsumer("CollisionCalculator");
	//create tcp server 
	TcpServer<std::string> tcpServer(TCP_LISTEN_PORT, dataM) ;
	Calculator flight = Calculator("FlightCalculator",dataM);
	//launch tcp server / data manager / calculator (consumer) in different threads
	std::thread proxyTh(&TcpServer<std::string>::run, &tcpServer); 
	std::thread dataMTh(&dataManager<std::string>::manage, dataM);
	std::thread flightTh(&Calculator::print, &flight); 

	flightTh.join();
	dataMTh.join();
	proxyTh.join();

	return 0;
}