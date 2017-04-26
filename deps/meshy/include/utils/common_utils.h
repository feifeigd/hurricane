#pragma once

#include <fcntl.h>
#include <cstdint>

namespace meshy {
	/// @return 0 ³É¹¦
	int32_t SetNonBlocking(int32_t sockfd);
}
