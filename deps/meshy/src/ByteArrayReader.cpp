#include "utils/exendian.h"

#include <ByteArrayReader.h>

using meshy::ByteArrayReader;
using meshy::ByteArray;

ByteArrayReader::ByteArrayReader(ByteArray const& buffer)
	: m_buffer(buffer), m_pos(0)
{

}

int16_t ByteArrayReader::readInt16BE() {
	int16_t value = read<int16_t>();
	return ConvertNetworkToHostShort(value);
}

int32_t ByteArrayReader::readInt32BE() {
	int32_t value = read<int32_t>();
	return ConvertNetworkToHostLong(value);
}

int64_t ByteArrayReader::readInt64BE() {
	int64_t value = read<int64_t>();
	return ConvertNetworkToHostLongLong(value);
}

ByteArray ByteArrayReader::ReadData(size_t size) {
	if (m_pos >= m_buffer.size())return 0;
	size_t sizeToRead = size;
	if (m_pos + sizeToRead > m_buffer.size())sizeToRead = m_buffer.size() - m_pos;
	ByteArray result(m_buffer.data() + m_pos, sizeToRead);
	m_pos += sizeToRead;
	return result;
}

size_t ByteArrayReader::tell()const {
	return m_pos;
}

void ByteArrayReader::seek(SeekMode mode, size_t size) {
	if (mode == SeekMode::Set)m_pos = size;
	else if (mode == SeekMode::Forward) m_pos += size;
	else if (mode == SeekMode::Backward) m_pos -= size;
}
