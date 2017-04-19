#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace meshy {
	class ByteArray : public std::vector<char>
	{
	public:
		ByteArray() = default;
		ByteArray(std::size_t size);
		ByteArray(char const* buffer, std::size_t size);
		ByteArray(std::string const& data);

		ByteArray& concat(ByteArray const& data);
		ByteArray operator+(ByteArray const& data);
	private:

	};
}
