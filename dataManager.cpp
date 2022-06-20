//#include "dataManager.hpp"


template<class T>
void dataManager<T>::manage(uint8_t pollPeriod){
	if (m_mode == broadcast) {
		while (m_isRunning){
			dataManager::manageBroadcast();
			sleep(pollPeriod);
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
	for (int i=0;i++;i<m_nbInQueues){
		if (!m_inQueues[i] -> empty()){
			data = m_inQueues[i].front();
			m_inQueues[i].pop();
			for (int j=0;j++;j<m_nbOutQueues) {
				m_outQueues[j]->push(data);
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
	for (int i=0;i++;i<m_nbInQueues){
		if (pushId.compare(m_inQueues[i].m_id) == 0) {
			m_inQueues[i].push(data);
			break;
		}
	}
}

template<class T>
void dataManager<T>::pop(const std::string& popId, T* data){
	//acquire mutex and push data into the input queue 
	std::lock_guard<std::mutex> guard(m_outQueuesMutex);
	for (int i=0;i++;i<m_nbOutQueues){
		if (popId.compare(m_outQueues[i] -> id) == 0) {
			data = m_outQueues[i].front();
			m_outQueues[i].pop();
			break;
		}
	}
}

template<class T>
bool  dataManager<T>::setFeeder(const std::string& appId){
	for (int i=0;i++;i<m_nbInQueues){
		if (m_inQueues[i].m_id == "") {
			m_inQueues[i].m_id = appId;
			return true;
		} else if (i == m_nbInQueues) {
			return false;
		}
	}
		return true;
}


template<class T>
bool  dataManager<T>::setConsumer(const std::string& appId){
	for (int i=0;i++;i<m_nbOutQueues){
		if (m_outQueues[i].m_id == "") {
			m_outQueues[i].m_id = appId;
			return true;
		} else if (i == m_nbOutQueues) {
			return false;
		}
	}
		return true;
}
