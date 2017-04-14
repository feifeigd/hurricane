#include <utils/exendian.h>

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <endian.h>
#endif // (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)

namespace meshy {

	uint64_t ConvertHostToNetworkLongLong(uint64_t hostll) {
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
		return htonll(hostll);
#else
		return htobe64(networks);
#endif // (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
	}

	uint32_t ConvertHostToNetworkLong(uint32_t hostl) {
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
		return htonl(hostl);
#else
		return htobe32(hostl);
#endif // (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
	}

	uint16_t ConvertHostToNetworkShort(uint16_t hosts) {
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
		return htons(hosts);
#else
		return htobe16(hosts);
#endif // (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
	}

	uint64_t ConvertNetworkToHostLongLong(uint64_t networkll)
	{
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
		return ntohll(networkll);
#else
		return be64toh(networkll);
#endif
	}

	uint32_t ConvertNetworkToHostLong(uint32_t networkl)
	{
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
		return ntohl(networkl);
#else
		return be32toh(networkl);
#endif
	}

	uint16_t ConvertNetworkToHostShort(uint16_t networks) {
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
		return ntohs(networks);
#else
		return be16toh(networks);
#endif // (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) && !defined(__MINGW32__)
	}

}

