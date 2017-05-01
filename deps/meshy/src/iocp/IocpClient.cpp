
#include <iocp/IocpClient.h>
#include <utils/common_utils.h>

using meshy::IocpClient;
using meshy::IocpClientPtr;
using meshy::IocpStream;
using meshy::NativeSocket;

IocpClient::IocpClient(NativeSocket socket) : IocpStream(socket) {
	
}

void IocpClient::connect(std::string const& host, uint16_t port) {
	m_nativeSocketAddress = {0};
	m_nativeSocketAddress.sin_family = AF_INET;
	m_nativeSocketAddress.sin_addr.s_addr = inet_addr(host.c_str());
	m_nativeSocketAddress.sin_port = htons(port);
	NativeSocket fd = GetNativeSocket();
	meshy::SetNonBlocking(fd);
	::connect(fd, (sockaddr*)&m_nativeSocketAddress, sizeof(m_nativeSocketAddress));
}

IocpClientPtr IocpClient::Connect(std::string const& host, uint16_t port) {
	NativeSocket clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	IocpClientPtr client = IocpClientPtr(new IocpClient(clientSocket));
	client->connect(host, port);
	return client;
}
