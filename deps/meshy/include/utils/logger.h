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
		void _WriteLog(Priority priority, std::string const& log);
		void __stdcall WriteLog(Priority priority, char const* fmt, ...);

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
}

#define TRACE_DEBUG(...)	meshy::Logger::get().WriteLog(meshy::Priority::DEBUG, __VA_ARGS__)
#define TRACE_STATE(...)	meshy::Logger::get().WriteLog(meshy::Priority::STATE, __VA_ARGS__)
#define TRACE_INFO(...)		meshy::Logger::get().WriteLog(meshy::Priority::INFO, __VA_ARGS__)
#define TRACE_WARNING(...)	meshy::Logger::get().WriteLog(meshy::Priority::WARNING, __VA_ARGS__)
#define TRACE_ERROR(...)	meshy::Logger::get().WriteLog(meshy::Priority::Error, __VA_ARGS__)
