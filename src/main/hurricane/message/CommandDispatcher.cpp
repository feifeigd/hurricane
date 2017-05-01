#include <hurricane/message/CommandDispatcher.h>

using hurricane::message::Command;
using hurricane::message::CommandDispatcher;

CommandDispatcher& CommandDispatcher::OnCommand(Command::Type::Values type, Handler handler) {
	m_handlers[type] = handler;
	return *this;
}

void CommandDispatcher::Dispatch(Command const& command) {
	m_handlers[command.type()](command.args(), command.src());
}
