
#include <meshy.h>

#include <hurricane/base/NetAddress.h>
#include <hurricane/base/NetListener.h>
#include <hurricane/base/Node.h>
#include <hurricane/base/DataPackage.h>
#include <hurricane/message/CommandDispatcher.h>

#include <iostream>
#include <map>
#include <vector>

using hurricane::base::DataPackage;
using hurricane::base::NetAddress;
//using hurricane::base::NetListener;
using hurricane::base::Node;
using hurricane::message::Command;
using hurricane::message::CommandDispatcher;

using meshy::ByteArray;

// 核心
NetAddress const NIMBUS_ADDRESS{ "127.0.0.1", 6000 };

// 计算节点
std::map<std::string, NetAddress> const SUPERVISOR_ADDRESSES{
	{"s1", {"127.0.0.1", 7001}}
};

typedef std::vector<std::string> Tasks;

int main() {
	std::cout << "Nimbus started." << std::endl;

	//ITopology* topology = GetTopology();

	// 计算节点映射表
	std::map<std::string, Node> supervisors;

	// 记录每个节点上的消息源任务分配情况
	std::map<std::string, Tasks> spoutTasks;
	std::map<std::string, Tasks> boltTasks;

	// key对应了数据源或数据处理单元的任务名称和字段名称
	// value对应了Supervisor节点和执行器编号
	std::map<std::pair<std::string, std::string>, std::pair<Node, int> > fieldDestinations;

	CommandDispatcher dispatcher;
	dispatcher.OnCommand(Command::Type::Join, [](hurricane::base::Variants args, meshy::IStream* src) {
		std::string supervisorName = args[0].GetStringValue();
	});

	NetListener netListener(NIMBUS_ADDRESS);
	netListener.OnData([&](meshy::IStream* stream, char const* buffer, size_t size) {
		ByteArray receivedData(buffer, size);
		DataPackage receivedPackage;
		receivedPackage.deserialize(receivedData);
		Command command(receivedPackage);
		command.src(stream);
		dispatcher.Dispatch(command);
	});
	netListener.listen();

	return 0;
}
