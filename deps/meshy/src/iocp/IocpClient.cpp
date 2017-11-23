
#include <iocp/IocpClient.h>
#include <iocp/IocpLoop.h>
#include <utils/common_utils.h>
#include <utils/logger.h>
#include <WS2tcpip.h>

using meshy::IocpLoop;
using meshy::IocpClient;
using meshy::IocpClientPtr;
using meshy::IocpStream;
using meshy::NativeSocket;

IocpClient::IocpClient(NativeSocket socket) : IocpStream(socket) {
	IocpLoop::get();
}

void IocpClient::connect(std::string const& host, uint16_t port) {
	m_nativeSocketAddress = {0};
	m_nativeSocketAddress.sin_family = AF_INET;
	inet_pton(AF_INET, host.c_str(), &m_nativeSocketAddress.sin_addr);
	m_nativeSocketAddress.sin_port = htons(port);
	NativeSocket fd = Socket::CreateNativeSocket();
	SetNativeSocket(fd);
	if (::connect(GetNativeSocket(), (sockaddr*)&m_nativeSocketAddress, sizeof(m_nativeSocketAddress))) {
		TRACE_ERROR("%s: error=%d", __FUNCTION__, WSAGetLastError());
		return;
	}
	SetConnected(true);
	meshy::SetNonBlocking(fd);
}

void IocpClient::reconnect() {
	close();
	NativeSocket fd = Socket::CreateNativeSocket();
	SetNativeSocket(fd);
	if (::connect(GetNativeSocket(), (sockaddr*)&m_nativeSocketAddress, sizeof(m_nativeSocketAddress))) {
		TRACE_ERROR("%s: error=%d", __FUNCTION__, WSAGetLastError());
		return;
	}
	SetConnected(true);
	meshy::SetNonBlocking(fd);
}

IocpClientPtr IocpClient::Connect(std::string const& host, uint16_t port) {
	NativeSocket clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	IocpClientPtr client = IocpClientPtr(new IocpClient(clientSocket));
	client->connect(host, port);
	return client;
}
