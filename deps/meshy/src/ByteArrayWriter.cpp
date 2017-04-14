#include <utils/exendian.h>
#include <ByteArrayWriter.h>

using meshy::ByteArray;
using meshy::ByteArrayWriter;

size_t ByteArrayWriter::write(ByteArray const& buffer) {
	m_buffer.concat(buffer);
	return buffer.size();
}

size_t ByteArrayWriter::writeInt16BE(int16_t value) {
	return write(ConvertHostToNetworkShort(value));
}

size_t ByteArrayWriter::writeInt32BE(int32_t value) {
	return write(ConvertHostToNetworkLong(value));
}

size_t ByteArrayWriter::writeInt64BE(int64_t value) {
	return  write(ConvertHostToNetworkLongLong(value));
}

ByteArray const& ByteArrayWriter::ToByteArray()const {
	return m_buffer;
}
