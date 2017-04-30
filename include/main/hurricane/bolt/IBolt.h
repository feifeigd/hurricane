#pragma once

#include "../base/ITask.h"

namespace hurricane {

	namespace base {
		class OutputCollector;
		class Values;
	}

	namespace bolt {

		/// 消息处理器接口
		class IBolt : public base::ITask {
		public:
			virtual void prepare(base::OutputCollector& outputCollector) = 0;
			virtual void cleanup() = 0;
			virtual void execute(base::Values const& values) = 0;
			virtual IBolt* clone()const = 0;
		};
	}
}
