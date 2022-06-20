#include <plog/Log.h>
#include <queue>
#include <array>
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
#include <mutex>
#include "runnable.hpp"

/* This class helps managing data coming from one or several sources and copies it to one or several
   output queues.
*/

enum MODE { broadcast, map};

//wrapper around the std::queue class
template<class T>
class queueWrapper:public std::queue<T> {
	
	public:
		std::string m_id; //name of the app writing in the queue or reading from the queue

		queueWrapper(): std::queue<T>(),m_id(""){

		}
		
		~queueWrapper(){

		}

};

template<class T>
class dataManager: public runnable
{

public:
	dataManager(uint8_t inQueues,uint8_t outQueues): m_nbInQueues(inQueues),m_nbOutQueues(outQueues), m_mode(broadcast)
	{
		PLOG_DEBUG << "Constructor; building " << m_nbInQueues << " input queues and "
		           <<  m_nbOutQueues << " output queues";

		m_inQueues = new queueWrapper<T>[m_nbInQueues];
		m_inQueues = new queueWrapper<T>[m_nbOutQueues];
	}

	~dataManager(){
		delete [] m_inQueues;
		delete [] m_outQueues;
	}


	void push(T&& data, const std::string& pushId);
	void pop(const std::string& pushId, T* data);
	//connect a feeder (get an id) to an input queue, if available
	bool  setFeeder(const std::string& appId);
	//connect a consumer (get an id) to an output queue, if available
	bool  setConsumer(const std::string& appId); 


private:
	const uint8_t m_nbInQueues;
	const uint8_t m_nbOutQueues;

	queueWrapper<T> * m_inQueues;
	queueWrapper<T> * m_outQueues;
	// used to monitor when queue starts to be filling up quickly
	//have here a routing table variable or some mode 
	const MODE m_mode;

	//mutexes for protecting acess to input / output queues

	std::mutex m_inQueuesMutex;
	std::mutex m_outQueuesMutex;

	void manage(uint8_t pollPeriod);
	void manageBroadcast();
	void manageMap();


};

#include "dataManager.cpp"

#endif

