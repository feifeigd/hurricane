#pragma once

#include "concurrent_queue.h"

#include <functional>

#define MIN_THREADS 2

namespace meshy {
	/// 线程池
	template<typename Type>
	class ThreadPool
	{
		ThreadPool(ThreadPool const&) = delete;
		ThreadPool& operator=(ThreadPool const&) = delete;

	public:
		ThreadPool(size_t threads, std::function<void(Type& record)> handler)
			: m_shutdown(false), m_threads(threads), m_handler(handler)
		{
			if (m_threads < MIN_THREADS)
			{
				m_threads = MIN_THREADS;
			}
			for (size_t i = 0; i < m_threads; ++i)
			{
				m_workers.emplace_back([&] {
					Type record;
					while (!m_shutdown)
					{
						m_tasks.pop(record);
						m_handler(record);
					}
				});
			}
		}

		~ThreadPool() {
			m_shutdown = true;
			for (std::thread& worker : m_workers)
			{
				worker.join();
			}
		}

		void submit(Type const& record) {
			m_tasks.push(record);
		}

	private:
		bool								m_shutdown;
		size_t								m_threads;
		std::function<void(Type& record)>	m_handler;
		std::vector<std::thread>			m_workers;
		ConcurrentQueue<Type>				m_tasks;
	};
}
