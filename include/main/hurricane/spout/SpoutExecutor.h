#pragma once

#include "ISpout.h"
#include "../base/Executor.hpp"

namespace hurricane {

	namespace message { class SupervisorCommander; }
	namespace topology { class ITogology; }

	namespace spout {

		class SpoutOutputCollector;

		class SpoutExecutor : base::Executor<spout::ISpout>{
		public:
			SpoutExecutor();

			virtual void StopTask()override;
			void SetExecutorIndex(int executorIndex) { m_executorIndex = executorIndex; }
			void commander(message::SupervisorCommander* commander) { m_commander = commander; }


		protected:
			virtual void OnCreate()override;
			virtual void OnStop()override;

			void RandomDestination(SpoutOutputCollector* outputCollector);
			void GroupDestination(SpoutOutputCollector* outputCollector, int fieldIndex);

		private:
			topology::ITogology*			m_topology;
			bool							m_needToStop;
			message::SupervisorCommander*	m_commander;
			int								m_executorIndex;
		};
	}
}