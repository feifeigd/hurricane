#pragma once

namespace meshy {

	class IODevice
	{
	public:
		enum class SeekMode {
			Set,
			Forward,
			Backward,
		};
	};
}
