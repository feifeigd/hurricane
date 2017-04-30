#include <iostream>

#define HURRICANE_RELEASE 1
#define HURRICANE_MODE 1

#if HURRICANE_MODE == HURRICANE_RELEASE

//#include <temp/WordCountTopology.h>

//using hurricane::topology::ITopology;

#endif

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Wrong arguments" << std::endl;
		exit(-1);
	}

	std::string supervisorName(argv[1]);
	std::cout << "Supervisor " << supervisorName.c_str() << " started" << std::endl;

#if HURRICANE_MODE == HURRICANE_RELEASE
	//ITopology* topology = GetTopology();

#endif // HURRICANE_MODE == HURRICANE_RELEASE

	return 0;
}
