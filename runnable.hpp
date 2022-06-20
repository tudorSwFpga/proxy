#include <iostream>
#ifndef RUNNABLE_H
#define RUNNABLE_H

class runnable
{
public:
	runnable() : m_isRunning(true){
		std::cout << "Runnable constructor" << std::endl;
	}
	~runnable() {
		std::cout << "Runnable distructor" << std::endl;
	}

	void start();
	void stop();

protected:
	bool m_isRunning;

};

#endif