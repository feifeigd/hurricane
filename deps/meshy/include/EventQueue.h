#pragma once

#include <mutex>
#include <vector>
#include <memory>

namespace meshy {

	class BaseEvent;

	class EventQueue
	{
	public:
		EventQueue(size_t timeout = 0);
		
		void PostEvent(BaseEvent* event);
		std::shared_ptr<BaseEvent> GetEvent();
	private:
		std::vector<std::shared_ptr<BaseEvent> >	m_events;
		std::condition_variable						m_waitCondition;
		size_t										m_timeout;
		std::mutex									m_mutex;
	};
}
