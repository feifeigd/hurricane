#pragma once

#include <cstdint>
#ifdef OS_LINUX
using std::size_t;
#endif

namespace meshy {
	class IStream;
	class DataSink
	{
	public:
		virtual size_t OnDataIndication(IStream* stream, char const* buf, size_t bytes) = 0;
	};
}
