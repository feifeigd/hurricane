#pragma once

class MessageLoopManager {
	MessageLoopManager(MessageLoopManager const&) = delete;
	MessageLoopManager const& operator=(MessageLoopManager const&) = delete;
public:
	static MessageLoopManager& get() {
		static MessageLoopManager manager;
		return manager;
	}

	void Register(std::string const& name, MessageLoop* loop) {
		m_messageLoops.insert({ name, std::shared_ptr<MessageLoop>(loop) });
	}

	void PostMessage(std::string const& name, Message* message) {
		auto messageLoopPair = m_messageLoops.find(name);
		if (messageLoopPair != m_messageLoops.end())
		{
			messageLoopPair->second->post(message);
		}
	}
private:
	MessageLoopManager() {}

	std::map<std::string, std::shared_ptr<MessageLoop> > m_messageLoops;
};
