
#include <epoll/EpollLoop.h>
#include <epoll/EpollServer.h>

using meshy::EpollServer;
using meshy::EpollConnectionPtr;

EpollServer::EpollServer(DataSink* dataSink) : BasicServer<EpollConnectionPtr>(dataSink)
{
}

int32_t EpollServer::listen(std::string const& host, uint16_t port, int backlog){
	bind(host, port);

	NativeSocket listenfd = GetNativeSocket();
	int32_t errorCode = ::listen(listenfd, backlog);
	if (-1 == errorCode){
		TRACE_ERROR("Listen socket failed!");
		assert(false);
		return errorCode;
	}

	errorCode = EpollLoop::get().AddEpollEvents(EPOLLIN, listenfd);
	if(-1 == errorCode){
		TRACE_ERROR("FATAL epoll_ctl: listen_sock!");
		assert(false);
		return errorCode;
	}

	EpollLoop::get().AddServer(listenfd, this);
	return 0;
}

EpollConnectionPtr EpollServer::accept(){
	NativeSocket listenfd = GetNativeSocket();
	NativeSocket conn_sock;
	NativeSocketAddress remote = {0};
	socklen_t addrlen = sizeof remote;
	while( (conn_sock = ::accept(listenfd, (sockaddr*)&remote, (socklen_t*)&addrlen)) > 0 ){
		meshy::SetNonBlocking(conn_sock);
		if(-1 == EpollLoop::get().AddEpollEvents(EPOLLIN | EPOLLET, conn_sock)){
			TRACE_ERROR("epoll_ctl: add");
			exit(EXIT_FAILURE);
		}
		EpollConnectionPtr connection = std::make_shared<EpollConnection>(conn_sock);
		if(m_connectHandler)m_connectHandler(connection.get());
		return connection;
	}
	if(-1 == conn_sock){
		if(errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR){
			TRACE_ERROR("accept");
		}
	}
	return nullptr;
}

	
