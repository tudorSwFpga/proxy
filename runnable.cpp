#include "runnable.hpp"

void runnable::start(){
	m_isRunning = true;
}

void runnable::stop(){
	m_isRunning = false;
}