#include <socket.h>
#include <utils/logger.h>

using meshy::Socket;
using meshy::NativeSocket;
using meshy::SocketAddress;

#ifdef OS_WIN32
#pragma comment(lib, "Ws2_32.lib")	// closesocket
#endif // OS_WIN32

NativeSocket Socket::CreateNativeSocket() {
	NativeSocket fd = socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == fd)
	{
#ifdef OS_WIN32
		TRACE_ERROR("Create socket failed! errorno={0}", WSAGetLastError());
#else
		TRACE_ERROR("Create socket failed! errorno={0}", errno);
#endif
		exit(1);
	}
	return fd;
}

Socket::Socket(NativeSocket nativeSocket, SocketAddress const& address)
	: m_nativeSocket(nativeSocket), m_socketAddress(address)
{
	
}

Socket::~Socket() {
	close();
}

void Socket::close() {
	TRACE_ERROR("关闭连接socket={}", m_nativeSocket);
#ifdef OS_WIN32
	closesocket(m_nativeSocket);
#else
	close(m_nativeSocket);
#endif // OS_WIN32
	m_nativeSocket = -1;
}

NativeSocket Socket::GetNativeSocket()const {
	return m_nativeSocket;
}

void Socket::SetNativeSocket(NativeSocket nativeSocket) {
	m_nativeSocket = nativeSocket;
}

SocketAddress const& Socket::GetSocketAddress()const {
	return m_socketAddress;
}

int Socket::type()const {	
	return getsockopt(SOL_SOCKET, SO_TYPE);
}

int Socket::getsockopt(int level, int optname)const {
	int optval = 0, optlen = 0;
	int state = ::getsockopt(m_nativeSocket, level, optname, (char*)&optval, &optlen);
	if (state)
	{
		// 发生了错误
		return -1;
	}
	return optval;
}
int Socket::setsockopt(int level, int optname, int optval) {
	int optlen = sizeof(optval);
	int state = ::setsockopt(m_nativeSocket, level, optname, (char const*)&optval, optlen);
	assert(SOCKET_ERROR != state);
	return state;
}

int Socket::sendbuf()const {
	return getsockopt(SOL_SOCKET, SO_SNDBUF);
}
int Socket::recvbuf()const {
	return getsockopt(SOL_SOCKET, SO_RCVBUF);
}


int Socket::sendbuf(int new_size) {
	return setsockopt(SOL_SOCKET, SO_SNDBUF, new_size);
}
int Socket::recvbuf(int new_size) {
	return setsockopt(SOL_SOCKET, SO_RCVBUF, new_size);
}

int Socket::nodelay(bool value) {
	int new_value = value;
	return setsockopt(IPPROTO_TCP, TCP_NODELAY, new_value);
}

int Socket::ReuseAddr() {
	return setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
}
