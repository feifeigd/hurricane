#pragma once

#include "../loop.h"

namespace meshy {

	class EpollLoop : public Loop {
	public:
		static EpollLoop& get();

	protected:
		virtual void run()override;

	private:
		void EpollThread();
	};
}