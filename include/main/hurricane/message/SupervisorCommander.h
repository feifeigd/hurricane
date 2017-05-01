#pragma once

namespace hurricane {

	namespace base { class Values; }

	namespace message {
		class SupervisorCommander {
		public:

			void SendTuple(int taskIndex, base::Values const& values);

			void RandomDestination();
		};
	}
}