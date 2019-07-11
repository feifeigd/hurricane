#pragma once

#ifdef OS_WIN32
#include "win32/net_win32.h"
#elif defined(OS_BSD)
#include "bsd/net_bsd.h"
#elif defined(OS_LINUX)
#include "linux/net_linux.h"
#endif // OS_WIN32

#if defined(OS_BSD) || defined(OS_LINUX)
#include <unistd.h>
#include <netinet/in.h>
#endif

namespace meshy {
	/// 原始socket的封装
	class Socket {
	public:
		Socket(NativeSocket nativeSocket = 0, NativeSocketAddress const& address = {0});
		virtual ~Socket();

		NativeSocket GetNativeSocket()const;
		NativeSocketAddress const& GetNativeSocketAddress()const;
		void SetNativeSocketAddress(NativeSocketAddress const& address) {
			m_nativeSocketAddress = address;
		}

		static NativeSocket CreateNativeSocket();
	protected:
		void close();
		void SetNativeSocket(NativeSocket nativeSocket);
		/// 对于监听socket，表示绑定的地址，其他则表示对方的地址
		NativeSocketAddress	m_nativeSocketAddress;
	private:
		NativeSocket		m_nativeSocket;

	};
}
