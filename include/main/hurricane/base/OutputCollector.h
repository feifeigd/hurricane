#pragma once

#include "../message/SupervisorCommander.h"
#include <string>

namespace hurricane {
	namespace base {

		class Values;

		class OutputCollector {
		public:
			/// 发送策略
			struct Strategy {
				enum Values
				{
					Global = 0,
					Random,
					Group,
				};
			};

			OutputCollector(std::string const& src, int strategy);

			/// 发送一个元组
			virtual void emit(Values const& values);
			void commander(message::SupervisorCommander* commander);
			void SetTaskIndex(int taskIndex) { m_taskIndex = taskIndex; }
			void SetGroupField(int groupField) { m_groupField = groupField; }
			int GetGroupField()const { return m_groupField; }

			virtual void RandomDestination() = 0;
			virtual void GroupDestination() = 0;

		private:
			std::string									m_src;			///< 消息收集器来源的任务名称
			int											m_strategy;		///< 发送策略
			int											m_taskIndex;	///< 目标任务编号
			hurricane::message::SupervisorCommander*	m_commander;	///< 命令发送器
			int											m_groupField;	///< 用在分组策略中，指定了分组使用的字段编号
		};
	}
}