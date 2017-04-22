#pragma once

namespace meshy {
	class Loop
	{
	public:
		virtual ~Loop();

		void start();

	protected:
		virtual void run() = 0;
	};
}
