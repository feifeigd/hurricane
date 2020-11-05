#include <SocketAddress.h>

using namespace meshy;

SocketAddress::SocketAddress(NativeSocketAddress const& address) : nativeSocketAddress(address)
{

}

SocketAddress::SocketAddress(uint32_t address, uint16_t port) {
	nativeSocketAddress.sin_addr.S_un.S_addr = htonl(address);
	nativeSocketAddress.sin_port = htons(port);
}

SocketAddress::SocketAddress(std::string const& host, uint16_t port) {
#ifdef OS_WIN32
	inet_pton(AF_INET, host.c_str(), &nativeSocketAddress.sin_addr);
#elif defined(OS_LINUX)
	inet_aton(host.c_str(), &nativeSocketAddress.sin_addr);	// windows不存在这个函数
#endif // OS_WIN32

	nativeSocketAddress.sin_port = htons(port);
}