#pragma once

#include "SocketAddress.h"

namespace meshy {
	/// 原始socket的封装
	class Socket {
	public:
		Socket(NativeSocket nativeSocket = 0, SocketAddress const& address = {});
		virtual ~Socket();

		NativeSocket GetNativeSocket()const;
		SocketAddress const& GetSocketAddress()const;
		void SetSocketAddress(SocketAddress const& address) {
			m_socketAddress = address;
		}

		/// @return socket的类型是SOCK_STREAM还是SOCK_DGRAM
		int type()const;
		int sendbuf()const;
		int recvbuf()const;

		int sendbuf(int new_size);
		int recvbuf(int new_size);
		/// 关闭nagle算法，传输大文件时关闭，默认是开启状态
		/// @param value true关闭nagle算法，false开启nagle算法
		int nodelay(bool value);
		int ReuseAddr();
		static NativeSocket CreateNativeSocket();

		/// 如果optval不是int类型，需要修改这代码和返回值
		/// @param level: SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP
		int getsockopt(int level, int optname)const;
		int setsockopt(int level, int optname, int optval);
	protected:
		void close();
		void SetNativeSocket(NativeSocket nativeSocket);
		/// 对于监听socket，表示绑定的地址，其他则表示对方的地址
		SocketAddress	m_socketAddress;
	private:
		NativeSocket		m_nativeSocket;
	};
}
