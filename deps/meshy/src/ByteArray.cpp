#include <ByteArray.h>

using meshy::ByteArray;

ByteArray::ByteArray(size_t size) : std::vector<char>(size)
{

}

ByteArray::ByteArray(char const* buffer, size_t size) : std::vector<char>(buffer, buffer + size)
{

}

ByteArray::ByteArray(std::string const& data) : std::vector<char>(data.c_str(), data.c_str() + data.size())
{

}

ByteArray& ByteArray::concat(ByteArray const& data) {
	size_t oldsize = size();
	size_t appendsize = data.size();
	resize(oldsize + appendsize);
	memcpy(this->data() + oldsize, data.data(), appendsize);
	return *this;
}

ByteArray ByteArray::operator+(ByteArray const& data) {
	size_t size = this->size();
	size_t datasize = data.size();
	ByteArray result(size + datasize);
	memcpy(result.data(), this->data(), size);
	memcpy(result.data() + size, data.data(), datasize);
	return result;
}
