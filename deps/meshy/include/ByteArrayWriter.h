#pragma once

#include "ByteArray.h"

namespace meshy {
	class ByteArrayWriter {
	public:
		template<typename T>
		size_t write(T const* buffer, size_t count) {
			size_t sizeToWrite = sizeof(T) * count;
			ByteArray buffer2((char const*)buffer, sizeToWrite);
			m_buffer.concat(buffer2);
			return sizeToWrite;
		}

		template<typename T>
		size_t write(T const& value) {
			return write(&value, 1);
		}

		size_t write(ByteArray const& buffer);

		size_t writeInt16BE(int16_t value);
		size_t writeInt32BE(int32_t value);
		size_t writeInt64BE(int64_t value);

		ByteArray const& ToByteArray()const;
	private:
		ByteArray	m_buffer;
	};
}
