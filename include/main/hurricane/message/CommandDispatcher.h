#pragma once

#include "Command.h"

#include <functional>
#include <map>

namespace hurricane {
	namespace message {
		class CommandDispatcher {
		public:
			typedef std::function<void(base::Variants args, meshy::IStream* src)>	Handler;

			CommandDispatcher& OnCommand(Command::Type::Values type, Handler handler);
			void Dispatch(Command const& command);

		private:
			std::map<Command::Type::Values, Handler>	m_handlers;
		};
	}
}