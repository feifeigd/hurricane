#pragma once

#ifdef OS_WIN32
#include "win32/net_win32.h"
#elif defined(OS_BSD)
#include "bsd/net_bsd.h"
#elif defined(OS_LINUX)
#include "linux/net_linux.h"
#endif // OS_WIN32

#if defined(OS_BSD) || defined(OS_LINUX)
#include <unistd.h>
#include <netinet/in.h>
#endif

#include <cstdint>
#include <string>

namespace meshy {
	class SocketAddress {
	public:
		SocketAddress() = default;
		SocketAddress(NativeSocketAddress const& address);
		SocketAddress(uint32_t address, uint16_t port);

		/// @param host 英文句号分割的地址，如 127.0.0.1
		SocketAddress(std::string const& host, uint16_t port);
		NativeSocketAddress nativeSocketAddress{ AF_INET };	// 暂时只支持ipv4

		size_t size()const { return sizeof(nativeSocketAddress); }
		operator sockaddr* () { return (sockaddr*)& nativeSocketAddress; }
	};
}
