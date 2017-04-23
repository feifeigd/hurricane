#pragma once

#include <utils/concurrent_queue.h>

#include <string>
#include <fstream>

namespace meshy {

	enum class Priority
	{
		DEBUG,
		STATE,
		INFO,
		WARNING,
		Error,
	};

	class Logger
	{
		Logger& operator=(Logger const&) = delete;
		Logger(Logger const&) = delete;
	public:
		static Logger& get();
		void SetPriority(Priority priority);
		Priority GetPriority()const;
		void WriteLog(Priority priority, std::string const& log);

	private:
		Logger(Priority priority);
		~Logger();
		void InitializeFileStream();
		void WriteThread();

		ConcurrentQueue<std::string>	m_queue;
		std::ofstream					m_fileStream;
		Priority						m_priority;
		bool							m_shutdown;
	};

#define TRACE_DEBUG(content)	meshy::Logger::get().WriteLog(Priority::DEBUG, content)
#define TRACE_STATE(content)	meshy::Logger::get().WriteLog(Priority::STATE, content)
#define TRACE_INFO(content)		meshy::Logger::get().WriteLog(Priority::INFO, content)
#define TRACE_WARNING(content)	meshy::Logger::get().WriteLog(Priority::WARNING, content)
#define TRACE_ERROR(content)	meshy::Logger::get().WriteLog(Priority::Error, content)
}
