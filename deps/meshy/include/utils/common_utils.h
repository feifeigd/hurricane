#pragma once

#include <fcntl.h>
#include <cstdint>

#ifdef OS_WIN32
#include <win32/net_win32.h>
#elif defined(OS_LINUX)
#include <linux/net_linux.h>
#endif // OS_WIN32

namespace meshy {
	/// @return 0 成功
	int32_t SetNonBlocking(NativeSocket sockfd);
}
