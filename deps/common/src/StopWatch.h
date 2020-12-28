#pragma once

#include <chrono>

class StopWatch
{
public:
	typedef	std::chrono::steady_clock clock;
	using microseconds = std::chrono::microseconds;
	using milliseconds = std::chrono::milliseconds;
	using seconds = std::chrono::seconds;

	StopWatch();

	clock::time_point restart();

	uint64_t elapsedUs()const;
	uint64_t elapsedMs()const;
	uint64_t elapsedSec()const;
private:
	clock::time_point m_start;

};

