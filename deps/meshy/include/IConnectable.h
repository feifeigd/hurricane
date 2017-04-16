#pragma once

#include <cstdint>
#include <string>

namespace meshy {
	class IConnectable
	{
	public:
		virtual void connect(std::string const& host, uint16_t port) = 0;
	};
}
