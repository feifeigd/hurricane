#include "StopWatch.h"

StopWatch::StopWatch()
	: m_start(clock::now())
{
	static_assert(clock::is_steady, "Serious OS/C++ library issues. Steady clock is not steady.");
}

StopWatch::clock::time_point StopWatch::restart() {
	m_start = clock::now();
	return m_start;
}


uint64_t StopWatch::elapsedUs()const {
	return std::chrono::duration_cast<microseconds>(clock::now() - m_start).count();
}

uint64_t StopWatch::elapsedMs()const {
	return std::chrono::duration_cast<milliseconds>(clock::now() - m_start).count();
}

uint64_t StopWatch::elapsedSec()const {
	return std::chrono::duration_cast<seconds>(clock::now() - m_start).count();
}
