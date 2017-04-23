
#include <epoll/EpollServer.h>

using meshy::EpollServer;
using meshy::EpollConnectionPtr;

int32_t EpollServer::bind(std::string const& host, uint16_t port){
	return 0;
}

EpollConnectionPtr EpollServer::accept(NativeSocket sockfd){
	return nullptr;
}

	
