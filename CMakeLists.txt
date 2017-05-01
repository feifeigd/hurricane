
cmake_minimum_required(VERSION 3.0)

project(hurricane)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
	${PROJECT_SOURCE_DIR}/deps/meshy/include
)

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

add_executable(nimbus src/main/hurricane/SupervisorLauncher.cpp
	include/main/hurricane/base/Executor.hpp	
	include/main/hurricane/base/OutputCollector.h	
	include/main/hurricane/base/Value.h	
	include/main/hurricane/base/Values.h
	include/main/hurricane/message/Message.h
	include/main/hurricane/message/MessgeLoopManager.h
	include/main/hurricane/spout/SpoutExecutor.h
	include/main/hurricane/spout/SpoutOutputCollector.h
	include/main/hurricane/message/SupervisorCommander.h
	src/main/hurricane/base/OutputCollector.cpp
	src/main/hurricane/base/Value.cpp
	src/main/hurricane/base/Values.cpp
	src/main/hurricane/spout/SpoutExecutor.cpp
	src/main/hurricane/spout/SpoutOutputCollector.cpp
	src/main/hurricane/message/MessageLoop.cpp
	src/main/hurricane/message/SupervisorCommander.cpp
)
target_link_libraries(nimbus meshy)
