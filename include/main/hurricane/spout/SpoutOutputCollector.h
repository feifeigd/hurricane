#pragma once

#include "../base/OutputCollector.h"

namespace hurricane {
	namespace spout {

		class SpoutOutputCollector : public base::OutputCollector{
		public:
			SpoutOutputCollector(std::string const& src, int strategy);

			virtual void RandomDestination()override;
			virtual void GroupDestination()override;
		};
	}
}