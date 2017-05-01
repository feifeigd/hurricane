
#ifdef OS_WIN32
#include <Windows.h>
#endif // OS_WIN32

#include <string>

namespace hurricane {

	namespace topology { class ITopology; }

	namespace base {
		using hurricane::topology::ITopology;
		typedef ITopology* (*TopologyGetter)();
#ifdef OS_WIN32
		ITopology* LoadTopology(std::string const& fileName) {
			HMODULE hModule = LoadLibrary(fileName.c_str());
			if (!hModule)return nullptr;
			TopologyGetter GetTopology = (TopologyGetter)GetProcAddress(hModule, "GetTopology");
			return GetTopology();
		}

#elif defined(OS_LINUX)
		topology::ITopology* LoadTopology(std::string const& fileName) {
			void* libm_handle = dlopen(fileName.c_str(), RTLD_LAZY);
			if (!libm_handle) {
				printf("Open Error:%s.\n", dlerror());
				return nullptr;
			}
			TopologyGetter GetTopology = dlsym(libm_handle, "GetTopology");
			char* errorInfo = dlerror();
			if (errorInfo) {
				printf("Dlsym Error:%s.\n", errorInfo);
				return nullptr;
			}
			ITopology* topology = GetTopology();
			dlclose(libm_handle);
			return topology;
		}

#endif // OS_WIN32
	}
}
