#pragma once

#include "loop.h"
#include <memory>

namespace meshy {

	class EventQueue;
	class BaseEvent;

	class EventQueueLoop : public Loop
	{
	public:
		EventQueueLoop(EventQueue* queue);

	protected:
		virtual void run()override;
		virtual void OnEvent(std::shared_ptr<BaseEvent> event) = 0;
	private:
		EventQueue*	m_queue;
	};
}
