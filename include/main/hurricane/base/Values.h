#pragma once

#include "Value.h"
#include <string>
#include <vector>

namespace hurricane {
	namespace base {

		/// 元组
		class Values : public std::vector<Value>{
		public:
			Values() = default;
			Values(std::initializer_list<Value> values);

		};
	}
}
