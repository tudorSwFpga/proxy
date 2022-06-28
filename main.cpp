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
	/* code */
	PLOG_INFO << "Starting System";
	//create data manager
	std::shared_ptr<dataManager<std::string>> dataM = std::make_shared<dataManager<std::string>>(1,2);
	//set interfaces for the data manager
	PLOG_INFO << "Setting data M interfaces";

	dataM->setFeeder("TCPServer");
	dataM->setConsumer("FlightCalculator");
	dataM->setConsumer("CollisionCalculator");
	//create tcp server 
	//std::shared_ptr<TcpServer> tcpServer = std::make_shared<TcpServer>(dataM);
	TcpServer tcpServer(dataM) ;

	Calculator flight = Calculator("FlightCalculator",dataM);

	//std::thread proxyTh(&TcpServer::run, tcpServer); 
	std::thread proxyTh(&TcpServer::run, &tcpServer); 
	std::thread flightTh(&Calculator::print, &flight); 
	std::thread dataMTh(&dataManager<std::string>::manage, dataM);

	flightTh.join();
	dataMTh.join();
	proxyTh.join();

	return 0;
}