
#include <ByteArray.h>
#include <epoll/EpollLoop.h>
#include <epoll/EpollStream.h>
//#include <utils/common_utils.h>
#include <utils/logger.h>

using meshy::EpollStream;

EpollStream::EpollStream(NativeSocket socket, NativeSocketAddress const& address)
	: BasicStream(socket, address)
{

}

size_t EpollStream::receive(char* buffer, size_t bufferSize, size_t& readSize){
	readSize = 0;
	int32_t nread = 0;
	NativeSocket fd = GetNativeSocket();
	while( (nread = read(fd, buffer + readSize, bufferSize - readSize)) > 0 ){
		readSize += nread;
	}
	return nread;
}

size_t EpollStream::send(ByteArray const& byteArray){
	TRACE_DEBUG("EpollStream::send");
	NativeSocket fd = GetNativeSocket();
	if(EpollLoop::get().ModifyEpollEvents(m_events | EPOLLOUT, fd)){
		TRACE_ERROR("FATAL epoll_ctl: mod failed!");
	}
	char const* buf = byteArray.data();
	size_t size = byteArray.size();
	size_t n = size;
	while(n > 0){
		int32_t nwrite = write(fd, buf + size - n, n);
		if(nwrite < n){
			if(nwrite == -1 && errno != EAGAIN){
				TRACE_ERROR("FATAL write data to peer failed!");
			}
			break;
		}
		n -= nwrite;
	}
	return 0;
}

uint32_t EpollStream::events()const{
	return m_events;
}

void EpollStream::events(uint32_t evts){
	m_events = evts;
}

