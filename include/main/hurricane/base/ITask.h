#pragma once

namespace hurricane {
	namespace base {
		/// 任务接口
		class ITask
		{
		public:
			virtual ~ITask();

			/// 声明的任务的字段名
			virtual Fields DeclareFields()const = 0;
		};
	}
}