#include <poll.h>


template<class T>
void dataManager<T>::manage(){
	if (m_mode == broadcast) {
		PLOG_DEBUG << " Manage data in broadcast mode";
		while (m_isRunning){
			PLOG_DEBUG << " App is running";
			dataManager::manageBroadcast();
			sleep(m_pollPeriod);
		}
	} else {
		dataManager::manageMap();
	}
}

template<class T>
void dataManager<T>::manageBroadcast(){
	//check each input queue status and copy on every output queue
	const std::lock_guard<std::mutex> lockin(m_inQueuesMutex);
	const std::lock_guard<std::mutex> lockout(m_outQueuesMutex);
	T data;
	for (int i=0;i<m_nbInQueues;i++){
		if (!m_inQueues[i].empty()){
			data = m_inQueues[i].front();
			m_inQueues[i].pop();
			for (int j=0;j<m_nbOutQueues;j++) {
				m_outQueues[j].push(data);
			}
		}
	}
}

template<class T>
void dataManager<T>::manageMap(){
}


template<class T>
void dataManager<T>::push(T&& data,const std::string& pushId){
	//acquire mutex and push data into the input queue 
	std::lock_guard<std::mutex> guard(m_inQueuesMutex);
	for (int i=0;i<m_nbInQueues;i++){
		if (pushId.compare(m_inQueues[i].m_id) == 0) {
			PLOG_DEBUG << " Pushed data in queue";
			m_inQueues[i].push(data);
			break;
		} else {
			PLOG_DEBUG << " Q ID did not match: " << pushId << " vs " << m_inQueues[i].m_id;		
		}
	}
	
}


template<class T>
bool dataManager<T>::pop(const std::string& popId, T* data){
	//acquire mutex and push data into the input queue 
	std::lock_guard<std::mutex> guard(m_outQueuesMutex);
	for (int i=0;i<m_nbOutQueues;i++){
		if (popId.compare(m_outQueues[i].m_id) == 0) {
			if (!m_outQueues[i].empty()) {
				data[0] = m_outQueues[i].front();
				m_outQueues[i].pop();
				return true;	
			} else {
				return false;
			}
			
		}
	}
	return false;
}

template<class T>
bool  dataManager<T>::setFeeder(const std::string& appId){
	for (int i=0;i<m_nbInQueues;i++){
		if (m_inQueues[i].m_id.size() == 0) {
			m_inQueues[i].m_id = appId;
			PLOG_DEBUG << " Feeder set to " << appId;
			return true;
		} else if (i == m_nbInQueues-1) {
			PLOG_DEBUG << " No more place to set Feeder ";
			return false;
		}
	}
		return true;
}


template<class T>
bool  dataManager<T>::setConsumer(const std::string& appId){
	for (int i=0;i<m_nbOutQueues;i++){
		if (m_outQueues[i].m_id.size() == 0) {
			m_outQueues[i].m_id = appId;
			PLOG_DEBUG << " Consumer ID set to " << appId;
			return true;
		} else if (i == m_nbOutQueues-1) {
			PLOG_DEBUG << " No more place to set Consumer ";
			return false;
		}
	}
		return true;
}
