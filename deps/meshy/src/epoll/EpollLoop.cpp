#include <epoll/EpollLoop.h>
#include <utils/logger.h>

#include <thread>

using meshy::EpollLoop;

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
}
