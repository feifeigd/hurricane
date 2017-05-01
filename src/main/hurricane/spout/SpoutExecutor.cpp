#include <hurricane/base/OutputCollector.h>
#include <hurricane/message/SupervisorCommander.h>
#include <hurricane/spout/SpoutExecutor.h>
#include <hurricane/spout/SpoutOutputCollector.h>

#include <iostream>

using hurricane::base::OutputCollector;

using hurricane::spout::SpoutExecutor;
using hurricane::spout::SpoutOutputCollector;

SpoutExecutor::SpoutExecutor() : m_topology(nullptr), m_needToStop(false), m_commander(nullptr), m_executorIndex(-1)
{

}

void SpoutExecutor::StopTask() {
	m_needToStop = true;
	Executor::StopTask();
}

void SpoutExecutor::OnCreate() {
	std::cout << "Start Spout Task" << std::endl;
	m_needToStop = false;
	SpoutOutputCollector* outputCollector;
	m_task->open(*outputCollector);

	while (!m_needToStop)
	{
		m_task->execute();
	}
	m_task->close();
}

void SpoutExecutor::OnStop() {
	std::cout << "Stop Spout Task" << std::endl;
}

void SpoutExecutor::RandomDestination(SpoutOutputCollector* outputCollector) {
	m_commander->RandomDestination();
}

void SpoutExecutor::GroupDestination(SpoutOutputCollector* outputCollector, int fieldIndex) {

}
