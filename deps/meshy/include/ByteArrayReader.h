#pragma once

#include "IODevice.hpp"
#include "ByteArray.h"

#include <string.h>

namespace meshy {
	class ByteArrayReader : public IODevice
	{
	public:
		ByteArrayReader(ByteArray const& buffer);

		template<typename T>
		size_t read(T* buffer, size_t count) {
			if (m_pos >= m_buffer.size())
				return 0;
			size_t sizeToRead = sizeof(T) * count;
			if (m_pos + sizeToRead > m_buffer.size())
				sizeToRead = m_buffer.size() - m_pos;
			memcpy(buffer, m_buffer.data() + m_pos, sizeToRead);
			m_pos += sizeToRead;
			return sizeToRead;
		}

		template<typename T>
		T read() {
			T t;
			size_t size = read(&t, 1);
			return t;
		}

		int16_t readInt16BE();
		int32_t readInt32BE();
		int64_t readInt64BE();

		ByteArray ReadData(size_t size);
		size_t tell()const;
		void seek(SeekMode mode, size_t size);
	private:
		ByteArray const& m_buffer;
		size_t				m_pos;
	};
}
