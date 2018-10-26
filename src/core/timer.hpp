#pragma once

#include "point.hpp"

#include <chrono>

namespace core {

class Timer
{
public:
	Timer()
		: m_startTime(std::chrono::high_resolution_clock::now())
	{}

	void reset()
	{
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	double getElapsed()
	{
		std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - m_startTime;
		return duration.count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> m_startTime;
};

}
