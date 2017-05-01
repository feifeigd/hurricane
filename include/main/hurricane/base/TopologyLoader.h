#pragma once

#include <string>

namespace hurricane {

	namespace topology { class ITopology; }

	namespace base {
		topology::ITopology* LoadTopology(std::string const& fileName);
	}
}
