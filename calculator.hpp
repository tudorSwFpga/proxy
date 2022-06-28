#include <plog/Log.h>
#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <mutex>
#include "dataManager.hpp"

/* This class helps managing data coming from one or several sources and copies it to one or several
   output queues.
*/

class Calculator: public runnable
{

public:
	//default constructor
	Calculator(std::string&& name,std::shared_ptr<dataManager<std::string>> dataHandler): m_name(name),m_dataHandler(dataHandler)
	{
		PLOG_INFO << " Constructing Calculator " << m_name;
	}

	~Calculator(){
	}

	void print(){
		std::string data;
		while (m_isRunning){
			if (m_dataHandler->pop(m_name,&data)){
				std::cout << "Calculator " << m_name <<  data << std::endl;	
			} 
			sleep(2);
		}
	}	

private:
	const std::string m_name;
	std::shared_ptr<dataManager<std::string>> m_dataHandler;

	
};

#endif

