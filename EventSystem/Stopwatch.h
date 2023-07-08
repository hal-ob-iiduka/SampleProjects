#pragma once
#include <chrono>

enum class TimePrecision
{
	Second,
	Milli,
};

class Stopwatch
{
public:

	void Start()
	{
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	void Stop()
	{
		m_stopTime = std::chrono::high_resolution_clock::now();
	}

	double GetRap(TimePrecision precision = TimePrecision::Second)
	{
		Stop();

		// rap 計測
		std::chrono::duration<double, std::milli> ms = m_stopTime - m_startTime;

		switch (precision)
		{
		case TimePrecision::Second: return (ms.count() / 1000.0);
		case TimePrecision::Milli:  return ms.count();
		default: break;
		}
	}

private:

	// * 最新フレーム更新時間
	std::chrono::high_resolution_clock::time_point m_startTime;
	std::chrono::high_resolution_clock::time_point m_stopTime;
};