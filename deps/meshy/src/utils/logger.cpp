#include <utils/logger.h>
#include <iostream>
#include <sstream>

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
	std::thread writeThread(func);
	writeThread.detach();
}

Logger& Logger::get() {
	static Logger logger(Priority::DEBUG);
	return logger;
}

Logger::~Logger() {
	m_shutdown = true;
}

void Logger::SetPriority(Priority priority) {
	m_priority = priority;
}

Priority Logger::GetPriority()const {
	return m_priority;
}

void Logger::InitializeFileStream() {
	std::string fileName("logs/log.log");
	m_fileStream = std::ofstream(fileName, std::ios_base::out | std::ios_base::trunc);
	if (!m_fileStream) {
		std::ostringstream ss_error;
		ss_error << "FATAL ERROR: could not Open log file: [" << fileName << "]"
			<< "\n\t\t std::ios_base state=" << m_fileStream.rdstate();
		std::cerr << ss_error.str().c_str() << std::endl;
	}
}

void Logger::WriteLog(Priority priority, std::string const& log) {
	if (priority < m_priority)return;
	std::stringstream ss;
	//ss << HurricaneUtils::GetCurrentTimeStamp();
	ss << " [" << PRIORITY_STRING[size_t(priority)] << "]" << log;
	m_queue.push(ss.str());
}

void Logger::WriteThread() {
	while (!m_shutdown)
	{
		std::string log;
		m_queue.pop(log);
		std::cout << log << std::endl;
		m_fileStream << log << std::endl;
	}
}