#pragma once

#include <fcntl.h>
#include <cstdint>

namespace meshy {
	int32_t SetNonBlocking(int32_t sockfd);
}
