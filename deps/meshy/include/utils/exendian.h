#pragma once

#include <cstdint>

namespace meshy {
	// host to network
	uint64_t ConvertHostToNetworkLongLong(uint64_t hostll);
	uint32_t ConvertHostToNetworkLong(uint32_t hostl);
	uint16_t ConvertHostToNetworkShort(uint16_t hosts);

	// network to host
	uint64_t ConvertNetworkToHostLongLong(uint64_t networkll);
	uint32_t ConvertNetworkToHostLong(uint32_t networkl);
	uint16_t ConvertNetworkToHostShort(uint16_t networks);
}
