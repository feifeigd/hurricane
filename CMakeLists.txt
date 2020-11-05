
cmake_minimum_required(VERSION 3.10)

include(cmake/HunterGate.cmake)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.214.tar.gz"
    SHA1 "e14bc153a7f16d6a5eeec845fb0283c8fad8c358"
)

project(hurricane)

set (CMAKE_CXX_STANDARD 17)

include_directories(
	${PROJECT_SOURCE_DIR}/deps/meshy/include
)

# 第三方库
include(cmake/third.cmake)

if(UNIX)
	add_definitions(-DOS_LINUX)
else()
	add_definitions(-DOS_WIN32)
endif()

add_subdirectory(
	deps/meshy
)

include_directories(include/main)
add_executable(hurricane main.cpp)
target_link_libraries(hurricane meshy)

set(COMMON_SRC 
	include/main/hurricane/base/DataPackage.h
	include/main/hurricane/base/IntWritable.h
	include/main/hurricane/base/NetAddress.h
	include/main/hurricane/base/NetConnector.h
	include/main/hurricane/base/NetListener.h
	include/main/hurricane/base/Node.h
	include/main/hurricane/base/Executor.hpp	
	include/main/hurricane/base/OutputCollector.h
	include/main/hurricane/base/TopologyLoader.h
	include/main/hurricane/base/Value.h	
	include/main/hurricane/base/Values.h
	include/main/hurricane/base/Variant.h
	include/main/hurricane/message/Command.h
	include/main/hurricane/message/CommandDispatcher.h
	include/main/hurricane/message/Message.h
	include/main/hurricane/message/MessgeLoopManager.h
	include/main/hurricane/message/SupervisorCommander.h
	include/main/hurricane/spout/SpoutExecutor.h
	include/main/hurricane/spout/SpoutOutputCollector.h
	src/main/hurricane/base/DataPackage.cpp
	src/main/hurricane/base/NetConnector.cpp
	src/main/hurricane/base/NetListener.cpp
	src/main/hurricane/base/OutputCollector.cpp
	src/main/hurricane/base/TopologyLoader.cpp
	src/main/hurricane/base/Value.cpp
	src/main/hurricane/base/Values.cpp
	src/main/hurricane/base/Variant.cpp
	src/main/hurricane/message/Command.cpp
	src/main/hurricane/message/CommandDispatcher.cpp
	src/main/hurricane/message/MessageLoop.cpp
	src/main/hurricane/message/SupervisorCommander.cpp
	src/main/hurricane/spout/SpoutExecutor.cpp
	src/main/hurricane/spout/SpoutOutputCollector.cpp

)

if(UNIX)
	set(LIBS meshy pthread dl)
else()
	set(LIBS meshy Mswsock.lib)
endif()

# 核心，负责管理整个集群
add_executable(nimbus src/main/hurricane/NimbusLauncher.cpp ${COMMON_SRC})
target_link_libraries(nimbus ${LIBS})

# 每个计算节点一个负责与Nimbus通信，并调度本机上执行的任务
add_executable(supervisor src/main/hurricane/SupervisorLauncher.cpp ${COMMON_SRC})
target_link_libraries(supervisor ${LIBS})

add_subdirectory(test)
