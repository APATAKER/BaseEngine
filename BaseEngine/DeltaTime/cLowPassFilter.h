#pragma once

#include <vector>

class cLowPassFilter
{
public:
	cLowPassFilter();
	void addValue(double newValue);
	double getAverage(void);

	void startTime();
	void updateTime(double& deltaTime);

private:
	double last_time;
	std::vector<double> vecTimes;
	std::vector<double>::size_type m_NextLocation;

	// Pick a value that's about the number of frames...
	static const int DEFAULTVECTORSIZE = 50;

};