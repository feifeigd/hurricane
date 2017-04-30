#pragma once

#include "../base/ITask.h"

namespace hurricane {

	namespace { class OutputCollector; }

	namespace spout {

		/// 消息源接口
		class ISpout : public base::ITask {
		public:
			/// 打开并初始化一个新的消息源
			virtual void open(base::OutputCollector& outputCollector) = 0;
			virtual void close() = 0;
			virtual void execute() = 0;
			virtual ISpout* clone()const = 0;
		};
	}
}