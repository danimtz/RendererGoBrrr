#pragma once

#include <iostream>
#include <chrono>


class Timer {
public:
	Timer()
	{
		m_startTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		stopTimer();
	}

	void stopTimer()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
		auto duration = end - start;

		double ms = duration * 0.001;
		double fps = 1000/ms;
		std::cout << duration << "us ( " << ms << "ms), FPS: " << fps << "\n";
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;

};	