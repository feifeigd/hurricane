
#include <epoll/EpollConnection.h>

using meshy::EpollConnection;

EpollConnection::EpollConnection(NativeSocket socket, NativeSocketAddress const& address)
	: EpollStream(socket, address)
{

}

