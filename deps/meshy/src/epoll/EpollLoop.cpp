#include <epoll/EpollConnection.h>
#include <epoll/EpollLoop.h>
#include <epoll/EpollServer.h>
#include <utils/logger.h>

#include <thread>

using meshy::EpollLoop;
using meshy::EpollServer;

EpollLoop& EpollLoop::get() {
	static EpollLoop epollLoop;
	return epollLoop;
}

void EpollLoop::run() {
	auto func = std::bind(&EpollLoop::EpollThread, this);
	std::thread listenThread(func);
	listenThread.detach();
}

void EpollLoop::EpollThread() {
	TRACE_DEBUG(__FUNCTION__);
	NativeSocketEvent events[MAX_EVENT_COUNT];
	while(!m_shutdown){
		int32_t nfds = epoll_wait(m_eventfd, events, MAX_EVENT_COUNT, -1);
		if(-1 == nfds){
			TRACE_ERROR("FATAL epoll_wait failed!");
			exit(EXIT_FAILURE);
		}
		HandleEvent(events, nfds);
	}
}

void EpollLoop::HandleEvent(NativeSocketEvent* events, int32_t nfds){
	for(int i = 0; i < nfds; ++i){
		NativeSocketEvent const& event = events[i];
		int32_t fd =event.data.fd;
		if(m_servers.find(fd) != m_servers.end()){
			accept(fd);
			continue;
		}
		if(event.events & EPOLLIN){
			read(fd, event.events);
		}
		if(event.events & EPOLLOUT){

		}
	}
}

void EpollLoop::accept(NativeSocket listenfd){
	TRACE_DEBUG(__FUNCTION__);
	EpollServer* server = m_servers.find(listenfd)->second;
	EpollConnectionPtr connection = server->accept(m_eventfd);
	if(connection) m_streams[connection->GetNativeSocket()] = connection;
}

void EpollLoop::read(int32_t fd, uint32_t events){
	
}

