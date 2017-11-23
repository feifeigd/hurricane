#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace meshy {

	/// 线程安全的消息队列
	template<typename Type>
	class ConcurrentQueue
	{
		ConcurrentQueue& operator=(ConcurrentQueue const&) = delete;
		ConcurrentQueue(ConcurrentQueue const&) = delete;
	public:
		ConcurrentQueue() {

		}
		~ConcurrentQueue() {
			m_condition.notify_all();
		}
		void push(Type const& record) {
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.push(record);
			m_condition.notify_one();
		}

		bool pop(Type& record, bool isBlocked = true) {
			if (isBlocked)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				while (m_queue.empty())
				{
					m_condition.wait(lock);
				}
			}
			else {
				std::lock_guard<std::mutex> lock(m_mutex);
				if (m_queue.empty())return false;
			}

			std::lock_guard<std::mutex> lock(m_mutex);
			if (m_queue.empty())return false;
			record = std::move(m_queue.front());
			m_queue.pop();
			return true;
		}

		size_t size()const {
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_queue.size();
		}

		bool empty()const {
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_queue.empty();
		}
	private:
		std::queue<Type>		m_queue;
		mutable std::mutex		m_mutex;
		std::condition_variable	m_condition;
	};
}
