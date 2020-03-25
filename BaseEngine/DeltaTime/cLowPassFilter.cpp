#include "cLowPassFilter.h"

#include <GLFW/glfw3.h>

cLowPassFilter::cLowPassFilter()
{
	// Init the vector
	for ( int count = 0; count != cLowPassFilter::DEFAULTVECTORSIZE; count++ )
	{
		this->vecTimes.push_back( 0.0 );
	}
	this->m_NextLocation = 0;

	return;
}

void cLowPassFilter::addValue(double newValue)
{
	this->vecTimes[this->m_NextLocation] = newValue;
	this->m_NextLocation++;

	// Have I gone off the end of the vector
	if (this->m_NextLocation >= this->vecTimes.size())
	{
		this->m_NextLocation = 0;
	}
	return;
}
double cLowPassFilter::getAverage(void)
{
	double total = 0.0;
	for (int index = 0; index != this->DEFAULTVECTORSIZE; index++)
	{
		total += this->vecTimes[index];
	}
	total = total / (double)this->DEFAULTVECTORSIZE;
	return total;
}

void cLowPassFilter::startTime()
{
	current_time = glfwGetTime();
}

void cLowPassFilter::updateTime(double &deltaTime)
{
	double new_time = glfwGetTime();
	// Frame time... (how many seconds since last frame)
	deltaTime = new_time - current_time;
	current_time = new_time;

	double framerate = 1.f / 60.f;
	//while(deltaTime > 0.0)
	//{
	//	float dt = (deltaTime < framerate) ? static_cast<float>(deltaTime) : framerate;
	//	deltaTime -= dt;
	//}
	//const double SOME_HUGE_TIME = 0.1;	// 100 ms;
	if (deltaTime > framerate)
	{
		deltaTime = framerate;
	}
}

