#pragma once

#include "ISpout.h"
#include "../base/Executor.hpp"

namespace hurricane {

	namespace topology { class ITogology; }

	namespace spout {
		class SpoutExecutor : base::Executor<spout::ISpout>{
		public:
			SpoutExecutor();

			virtual void StopTask()override;
		private:
			topology::ITogology*	m_topology;
			bool					m_needToStop;
		};
	}
}