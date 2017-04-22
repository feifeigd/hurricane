#include <DataSink.h>
#include <epoll/EpollConnection.h>
#include <epoll/EpollLoop.h>
#include <epoll/EpollServer.h>
#include <utils/logger.h>

#include <thread>
#include <cassert>
#include <string.h>
#include <csignal>

using meshy::DataSink;
using meshy::EpollLoop;
using meshy::EpollServer;
using meshy::EpollStreamPtr;

EpollLoop& EpollLoop::get() {
	static EpollLoop epollLoop;
	return epollLoop;
}

EpollLoop::~EpollLoop(){
	m_shutdown = true;
}

void EpollLoop::AddServer(NativeSocket socket, EpollServer* server){
	m_servers.insert({socket, server});
}

void EpollLoop::AddStream(EpollStreamPtr stream){
	m_streams[stream->GetNativeSocket()] = stream;
}

int32_t EpollLoop::AddEpollEvents(int32_t events, NativeSocket fd){
	NativeSocketEvent ev;
	ev.events = events;
	ev.data.fd = fd;
	return epoll_ctl(m_eventfd, EPOLL_CTL_ADD, fd, &ev);
}

int32_t EpollLoop::ModifyEpollEvents(int32_t events, NativeSocket fd){
	NativeSocketEvent ev;
	ev.events = events;
	ev.data.fd = fd;
	return epoll_ctl(m_eventfd, EPOLL_CTL_MOD, fd, &ev);
}

EpollLoop::EpollLoop() : m_shutdown(false){
	TRACE_DEBUG(__FUNCTION__);
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGPIPE);
	sigprocmask(SIG_BLOCK, &set, nullptr);
	initialize();
}

void EpollLoop::run() {
	auto func = std::bind(&EpollLoop::EpollThread, this);
	std::thread listenThread(func);
	listenThread.detach();
}

void EpollLoop::initialize(){
	m_eventfd = epoll_create(MAX_EVENT_COUNT);
	if(-1 == m_eventfd){
		TRACE_ERROR("FATAL epoll_create failed!");
		assert(0);
	}
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
		int32_t fd = event.data.fd;
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

void EpollLoop::read(NativeSocket fd, uint32_t events){
	TRACE_DEBUG(__FUNCTION__);
	EpollStreamPtr stream = m_streams[fd];
	char buffer[BUFSIZ];
	size_t readSize = 0;
	size_t nread = stream->receive(buffer, sizeof(buffer), readSize);
	if( (-1 == nread && errno != EAGAIN) || readSize == 0){
		m_streams.erase(fd);
		char message[64];
		sprintf(message, "errno: %d: %s, nread: %lu, n: %lu", errno, strerror(errno), nread, readSize);
		TRACE_WARNING(message);
		return;
	}

	enqueue(stream, buffer, readSize);
}

void EpollLoop::enqueue(EpollStreamPtr stream,char const* buf, size_t nread){
	TRACE_DEBUG(__FUNCTION__);
	if(stream->GetDataSink()){
		TRACE_DEBUG("enqueue, datasink registered.");
		stream->GetDataSink()->OnDataIndication(stream.get(),  buf, nread);
	}
}

