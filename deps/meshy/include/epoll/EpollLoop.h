#pragma once

#include "../loop.h"
#include "../linux/common.h"
#include "../linux/net_linux.h"

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
