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
		/// 收到了对端的一段数据
		virtual size_t OnDataIndication(IStream* stream, char const* buf, size_t bytes) = 0;
	};
}
