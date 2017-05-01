#include <iocp/WSAConnection.h>

using meshy::NativeSocket;
using meshy::NativeSocketAddress;
using meshy::WSAConnection;

WSAConnection::WSAConnection(NativeSocket socket, NativeSocketAddress const& address) : IocpStream(socket, address)
{

}
