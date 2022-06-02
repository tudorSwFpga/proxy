#include <iostream>


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