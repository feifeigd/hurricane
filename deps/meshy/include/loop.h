#pragma once

namespace meshy {
	class Loop
	{
	public:
		virtual ~Loop();

		void star();

	protected:
		virtual void run() = 0;
	};
}
