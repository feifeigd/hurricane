#pragma once

#include <utils/concurrent_queue.h>
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/ostream.h>
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

		template<typename S, typename... Args>
		void WriteLog(char const* filename, int line,Priority priority, S&& fmt, Args&&... args) {
			if (priority < m_priority)return;
			filename = strrchr(filename, '\\') + 1;
			std::string msg = fmt::format("{}-{}({}):", std::this_thread::get_id(), filename, line);
			msg.append(fmt::format(fmt, args...)); 
			return _WriteLog(priority, msg);
		}		

	private:
		Logger(Priority priority);
		~Logger();
		void InitializeFileStream();
		void WriteThread();
		void _WriteLog(Priority priority, std::string const& log);

		ConcurrentQueue<std::string>	m_queue;
		std::ofstream					m_fileStream;
		std::thread						m_writeThread;
		Priority						m_priority;
		bool							m_shutdown;
	};
}

#define TRACE_DEBUG(...)	meshy::Logger::get().WriteLog(__FILE__, __LINE__, meshy::Priority::DEBUG, __VA_ARGS__)
#define TRACE_STATE(...)	meshy::Logger::get().WriteLog(__FILE__, __LINE__, meshy::Priority::STATE, __VA_ARGS__)
#define TRACE_INFO(...)		meshy::Logger::get().WriteLog(__FILE__, __LINE__, meshy::Priority::INFO, __VA_ARGS__)
#define TRACE_WARNING(...)	meshy::Logger::get().WriteLog(__FILE__, __LINE__, meshy::Priority::WARNING, __VA_ARGS__)
#define TRACE_ERROR(...)	meshy::Logger::get().WriteLog(__FILE__, __LINE__, meshy::Priority::Error, __VA_ARGS__)
