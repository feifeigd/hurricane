
#include <epoll/EpollConnection.h>

using meshy::EpollConnection;

EpollConnection::EpollConnection(NativeSocket socket)
	: EpollStream(socket)
{

}

