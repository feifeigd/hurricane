#pragma once

#include <vector>

namespace meshy {
	class ByteArray : public std::vector<char>
	{
	public:
		ByteArray() = default;
		ByteArray(size_t size);
		ByteArray(char const* buffer, size_t size);
		ByteArray(std::string const& data);

		ByteArray& concat(ByteArray const& data);
		ByteArray operator+(ByteArray const& data);
	private:

	};
}
