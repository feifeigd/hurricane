
//#include <meshy.h>
#include <hurricane/base/NetAddress.h>
#include <hurricane/message/SupervisorCommander.h>

#include <iostream>
#include <thread>

using hurricane::base::NetAddress;
using hurricane::message::SupervisorCommander;

//#include <temp/WordCountTopology.h>

//using hurricane::topology::ITopology;
NetAddress const NIMBUS_ADDRESS{ "127.0.0.1", 6000 };

void AliveThreadMain(std::string const& name) {
	SupervisorCommander commander(NIMBUS_ADDRESS, name);
	commander.join();
	while (true) {
		commander.alive();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Wrong arguments" << std::endl;
		exit(-1);
	}

	std::string supervisorName(argv[1]);
	std::cout << "Supervisor " << supervisorName.c_str() << " started" << std::endl;

	//ITopology* topology = GetTopology();
	std::thread aliveThread(AliveThreadMain, supervisorName);

	return 0;
}
