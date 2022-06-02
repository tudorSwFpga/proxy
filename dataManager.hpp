#include <plog/Log.h>
#include <queue>
#include <mutex>
#include "runnable.hpp"

/* This class helps managing data coming from one or several sources and copies it to one or several
   output queues.
*/

enum MODE { broadcast, map};

template<class T> class queueWrapper;

template<class T>
class dataManager: public runnable
{

using dataVector_t = queueWrapper<T>;

public:
	dataManager(uint8_t inQueues,uint8_t outQueues): m_nbInQueues(inQueues),m_nbOutQueues(outQueues)
	{
		PLOG_DEBUG << "Constructor; building " << m_nbInQueues << " input queues and "
		           <<  m_nbOutQueues << " output queues";

		m_inQueues = new dataVector_t[m_nbInQueues];
		m_inQueues = new dataVector_t[m_nbOutQueues];
	}

	~dataManager(){
		delete [] m_inQueues;
		delete [] m_outQueues;
	}


	void push(T& data, std::string& pushId);
	void pop(std::string& pushId, T* data);

private:
	const uint8_t m_nbInQueues;
	const uint8_t m_nbOutQueues;
	const uint8_t m_queueDepth; 

	dataVector_t * m_inQueues;
	dataVector_t * m_outQueues;
	// used to monitor when queue starts to be filling up quickly
	//have here a routing table variable or some mode 
	const MODE m_mode;

	void manage(uint8_t pollPeriod);
	void manageBroadcast();
	void manageMap();
	//mutexes for protecting acess to input / output queues
	std::mutex m_inQueuesMutex;
	std::mutex m_outQueuesMutex;

};


//wrapper around the std::queue class
template<class T>
class queueWrapper:public std::queue<T> {
	
	public:
		std::string id; //name of the app writing in the queue or reading from the queue

		queueWrapper(std::string queueId): std::queue<T>(),id(queueId){

		}
		
		~queueWrapper(){

		}


};