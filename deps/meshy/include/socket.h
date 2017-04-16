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
	class Socket {
	public:
		Socket(NativeSocket nativeSocket);
		virtual ~Socket();

		NativeSocket GetNativeSocket()const;
		void SetNativeSocket(NativeSocket nativeSocket);
	private:
		NativeSocket m_nativeSocket;
	};
}
