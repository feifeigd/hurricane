#pragma once

#include "ByteArray.h"
#include <string>

namespace meshy {

	class IStream;
	class BaseEvent
	{
	public:
		BaseEvent(std::string const& type = "", ByteArray const& data = ByteArray(), IStream* stream = nullptr);

		void type(std::string const& type) { m_type = type; }
		std::string const& type()const { return m_type; }

		void data(ByteArray const& data) { m_data = data; }
		ByteArray const& data()const { return m_data; }

		void stream(IStream* stream) { m_stream = stream; }
		IStream* stream()const { return m_stream; }

	private:
		std::string	m_type;
		ByteArray	m_data;
		IStream*	m_stream;
	};
}
