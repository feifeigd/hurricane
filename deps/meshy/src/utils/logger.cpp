#include <utils/logger.h>
#include <utils/time.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <cstdarg>
#include <functional>
#include <experimental/filesystem>
#include <direct.h>
#include <io.h>

using meshy::Logger;
using meshy::Priority;

namespace meshy {
	std::string const PRIORITY_STRING[] = {
		"DEBUG",
		"STATE",
		"INFO",
		"WARNING",
		"ERROR",
	};

}

Logger::Logger(Priority priority) : m_priority(priority), m_shutdown(false)
{
	InitializeFileStream();
	auto func = std::bind(&Logger::WriteThread, this);
	//std::thread writeThread(func);
	//writeThread.detach();
	m_writeThread = std::thread{ func };
}

Logger& Logger::get() {
	static Logger logger(Priority::DEBUG);
	return logger;
}

Logger::~Logger() {
	m_shutdown = true;
	if(m_writeThread.joinable())
		m_writeThread.join();
}

void Logger::SetPriority(Priority priority) {
	m_priority = priority;
}

Priority Logger::GetPriority()const {
	return m_priority;
}

void Logger::InitializeFileStream() {	
	// 创建logs目录
	if (_access("logs", 06)) {
		_mkdir("logs");
	}
	std::string fileName("logs/log.log");
	m_fileStream.open(fileName, std::ios_base::out | std::ios_base::trunc);
	if (!m_fileStream) {
		std::ostringstream ss_error;
		ss_error << "FATAL ERROR: could not Open log file: [" << fileName << "]"
			<< "\n\t\t std::ios_base state=" << m_fileStream.rdstate();
		std::cerr << ss_error.str().c_str() << std::endl;
	}
}

/*void Logger::WriteLog(Priority priority, char const* fmt, ...)
{
	char log[4*1024];
	va_list args;
	va_start(args, fmt);
	_vsnprintf(log, sizeof log, fmt, args);
	va_end(args);
	return _WriteLog(priority, log);
}*/

void Logger::_WriteLog(Priority priority, std::string const& log) {
	if (priority < m_priority)return;
	std::stringstream ss;
	ss << HurricaneUtils::GetCurrentTimeStamp();
	ss << "[" << PRIORITY_STRING[size_t(priority)] << "]" << log;
	m_queue.push(ss.str());
}

void Logger::WriteThread() {
	while (!m_shutdown)
	{
		std::queue<std::string> logs;
		m_queue.pop_all(logs);
		std::string log;

		while (!m_shutdown && logs.size())
		{
			std::cout << logs.front() << std::endl;
			m_fileStream << logs.front() << std::endl;
			logs.pop();
		}
	}
}
