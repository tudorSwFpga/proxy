#include <plog/Log.h>
#include <queue>
#include <array>
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
#include <mutex>
#include <thread>
#include <map>
#include <condition_variable>
#include <functional>
#include <iterator>
#include "runnable.hpp"

/* This class helps managing data coming from one or several sources and copies it to one or several
   output queues.
*/

enum MODE { broadcast, map};


template<class T>
class dataManager: public runnable
{

public:
	dataManager(uint8_t inQueues,uint8_t outQueues): m_maxNbInQueues(inQueues),m_maxNbOutQueues(outQueues), m_mode(broadcast)
	{
		PLOG_DEBUG << "Constructor;";
	}

	~dataManager(){
		/*delete  m_inQueues;
		delete  m_outQueues;
		delete  m_inQueuesIds;
		delete  m_outQueuesIds;*/
	}


	void push(T&& data, const std::string& pushId);
	//T pop(const std::string& pushId);
	bool pop(const std::string& pushId, T* data);
	bool isNotEmpty(const std::string& popId);

	//connect a feeder (get an id) to an input queue, if available
	bool  setFeeder(const std::string& appId);
	//remove a feeder
	bool  remFeeder(const std::string& appId);
	//TODO: 
	//setDecoder
	//setParser

	//connect a consumer (get an id) to an output queue, if available
	bool  setConsumer(const std::string& appId);
	//remove a consumer
	bool  remConsumer(const std::string& appId);
	void  manage();


private:
	const int m_maxNbInQueues;
	const int m_maxNbOutQueues;

	std::vector<std::queue<T>> m_inQueues;
	std::vector<std::queue<T>> m_outQueues;

	std::map<std::string,int> m_inQueuesIds;
	std::map<std::string,int> m_outQueuesIds;
	//have here a routing table variable or some mode 
	const MODE m_mode;

	//mutexes for protecting acess to input / output queues
	std::mutex m_inQueuesMutex;
	std::condition_variable m_condVarIn;
	std::mutex m_outQueuesMutex;
	std::condition_variable m_condVarOut;
	//broadcast all the input data to output queues
	void manageBroadcast();
	//route input packets to output queues based on a routing table
	void manageMap();


};

#include "dataManager.cpp"

#endif

