#include <BaseEvent.h>

using meshy::BaseEvent;
using meshy::ByteArray;
using meshy::IStream;

BaseEvent::BaseEvent(std::string const& type, ByteArray const& data, IStream* stream)
	: m_type(type), m_data(data), m_stream(stream)
{
}