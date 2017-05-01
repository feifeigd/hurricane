#include <hurricane/spout/SpoutOutputCollector.h>

using hurricane::base::OutputCollector;
using hurricane::spout::SpoutOutputCollector;

SpoutOutputCollector::SpoutOutputCollector(std::string const& src, int strategy)
	: OutputCollector(src, strategy)
{

}

void SpoutOutputCollector::RandomDestination() {

}

void SpoutOutputCollector::GroupDestination() {

}