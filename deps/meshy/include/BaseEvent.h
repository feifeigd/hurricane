#pragma once

#include "ByteArray.h"
#include <string>

namespace meshy {

	class IStream;
	class BaseEvent
	{
	public:
		BaseEvent(std::string const& type = "", ByteArray const& data = ByteArray(), IStream* stream = nullptr);

	private:
		std::string	m_type;
		ByteArray	m_data;
		IStream*	m_stream;
	};
}
