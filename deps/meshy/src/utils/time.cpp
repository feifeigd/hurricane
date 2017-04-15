#include <utils/time.h>
#include <iomanip>	// std::put_time
#include <chrono>
#include <sstream>
#include <cassert>

using namespace std;
using namespace std::chrono;

namespace HurricaneUtils {
	std::string GetCurrentTimeStamp() {
		auto currentTime = system_clock::now();
		auto milliseconds = duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch() % 1000);
		auto currentTimePoint = system_clock::to_time_t(currentTime);
		std::ostringstream oss;
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
		oss << std::put_time(std::localtime(&currentTimePoint), "%T");
#else
		char buffer[80];
		auto success = std::strftime(buffer, sizeof(buffer), "%T", std::localtime(&currentTimePoint));
		assert(0 != success);
		oss << buffer;
#endif
		oss << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();
		return oss.str();
	}
}
