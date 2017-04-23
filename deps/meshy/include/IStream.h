#pragma once

#include <functional>

namespace meshy {

	class ByteArray;

	class IStream
	{
	public:
		typedef std::function<void(char const* buf, size_t size)> DataIndicationHandler;

		virtual size_t receive(char* buffer, size_t bufferSize, size_t& readSize) = 0;
		virtual size_t send(ByteArray const& byteArray) = 0;

		//virtual void OnDataIndication(DataIndicationHandler handler) = 0;
		//virtual DataIndicationHandler GetDataIndication() = 0;
	};
}
