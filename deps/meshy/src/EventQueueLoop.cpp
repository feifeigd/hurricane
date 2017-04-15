#include <EventQueueLoop.h>
#include <EventQueue.h>

using meshy::EventQueue;
using meshy::EventQueueLoop;

EventQueueLoop::EventQueueLoop(EventQueue* queue) : m_queue(queue)
{
}

void EventQueueLoop::run()
{
	while (true)	// 这个死循环如何解决？
	{
		std::shared_ptr<BaseEvent> event = m_queue->GetEvent();
		if (!event)continue;
		OnEvent(event);
	}
}
