#include <utils/common_utils.h>
#include <utils/logger.h>

namespace meshy {
#ifdef OS_LINUX
	int32_t SetNonBlocking(NativeSocket sockfd) {
		int32_t opts = fcntl(sockfd, F_GETFL);
		if (opts < 0)
		{
			TRACE_ERROR("fcntl(F_GETFL)\n");
			return -1;
		}
		opts |= O_NONBLOCK;
		if (fcntl(sockfd, F_SETFL, opts) < 0)
		{
			TRACE_ERROR("fcntl(F_SETFL)");
			return -1;
		}
		return 0;
	}
#elif defined(OS_WIN32)
	int32_t SetNonBlocking(NativeSocket sockfd) {
		u_long ul = 1;
		if (NO_ERROR != ioctlsocket(sockfd, FIONBIO, &ul)) {
			TRACE_ERROR("ioctlsocket");
			return -1;
		}
		return 0;
	}
#endif // OS_LINUX

}
