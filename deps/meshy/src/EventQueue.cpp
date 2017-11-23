#include <EventQueue.h>

#include <BaseEvent.h>

using meshy::EventQueue;
using meshy::BaseEvent;

EventQueue::EventQueue(size_t timeout) :m_timeout(timeout)
{
}

void EventQueue::PostEvent(BaseEvent* event) {
	std::unique_lock<std::mutex> lock(m_mutex);
	m_events.push_back(std::shared_ptr<BaseEvent>(event));
}

std::shared_ptr<BaseEvent> EventQueue::GetEvent() {
	std::unique_lock<std::mutex> lock(m_mutex);
	if (m_events.empty())
	{
		if (!m_timeout)return nullptr;
		m_waitCondition.wait_for(lock, std::chrono::milliseconds(m_timeout));
	}
	if (!m_events.empty())
	{
		std::shared_ptr<BaseEvent> event = m_events.front();
		m_events.erase(m_events.begin());
		return event;
	}
	return nullptr;
}
