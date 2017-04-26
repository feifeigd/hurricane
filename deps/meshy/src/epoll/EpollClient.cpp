
#include <epoll/EpollClient.h>
#include <epoll/EpollLoop.h>
#include <linux/net_linux.h>
#include <utils/common_utils.h>
#include <utils/logger.h>

using meshy::EpollClient;
using meshy::EpollLoop;
using meshy::EpollClientPtr;

EpollClient::EpollClient(NativeSocket socket)
	: EpollStream(socket)
{
}

void EpollClient::connect(std::string const& host, uint16_t port){
	NativeSocketAddress serv_addr = {0};
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(host.c_str());
	serv_addr.sin_port = htons(port);

	NativeSocket fd = GetNativeSocket();
	meshy::SetNonBlocking(fd);
	::connect(fd, (sockaddr*)&serv_addr, sizeof(serv_addr));
}

EpollClientPtr EpollClient::connect(std::string const& host, uint16_t port, DataSink* dataSink){
	NativeSocket fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	EpollClientPtr client = EpollClientPtr(new EpollClient(fd));
	client->SetDataSink(dataSink);
	client->connect(host, port);
	client->m_events = EPOLLIN | EPOLLET;
	if(-1 == EpollLoop::get().AddEpollEvents(client->m_events, fd)){
		TRACE_ERROR("epoll_ctl:add");
		exit(EXIT_FAILURE);
	}
	EpollLoop::get().AddStream(client);
	return client;
}

