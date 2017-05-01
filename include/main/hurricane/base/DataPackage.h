#pragma once

#include "Variant.h"

#include <meshy.h>

#include <memory>

namespace hurricane {
	namespace base {
		class DataPackage {
		public:
			void deserialize(meshy::ByteArray const& data);
		};
	}
}