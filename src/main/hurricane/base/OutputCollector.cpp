#include <hurricane/base/OutputCollector.h>
#include <hurricane/base/Values.h>

using hurricane::base::Values;
using hurricane::base::OutputCollector;
using hurricane::message::SupervisorCommander;

OutputCollector::OutputCollector(std::string const& src, int strategy) 
	: m_src(src), m_strategy(strategy), m_commander(nullptr), m_taskIndex(-1), m_groupField(-1)
{

}

void OutputCollector::emit(Values const& values) {
	if (Strategy::Global == m_strategy)
	{
		//if (m_commander)m_commander->SendTuple(values);
	}
	else if (Strategy::Random == m_strategy) {
		RandomDestination();
	}
	else if (Strategy::Group) {
		GroupDestination();
	}

	if (m_commander)m_commander->SendTuple(m_taskIndex, values);
}

void OutputCollector::commander(SupervisorCommander* commander) {
	delete m_commander;
	m_commander = commander;
}
