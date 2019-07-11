#pragma once

#include <cstdint>
#include <string>

namespace meshy {
	/// 对远程服务器发起连接
	class IConnectable
	{
	public:
		virtual void connect(std::string const& host, uint16_t port) = 0;
	};
}
