
set(COMMON_FILES
	include/BaseEvent.h
	include/BasicStream.h
	include/ByteArray.h
	include/ByteArrayReader.h
	include/ByteArrayWriter.h
	include/EventQueue.h
	include/EventQueueLoop.h
	include/loop.h
	include/socket.h
	include/utils/exendian.h
	include/utils/logger.h
	include/utils/time.h
	src/BaseEvent.cpp
	src/BasicStream.cpp
	src/ByteArray.cpp
	src/ByteArrayReader.cpp
	src/ByteArrayWriter.cpp
	src/EventQueue.cpp
	src/EventQueueLoop.cpp
	src/loop.cpp
	src/socket.cpp
	src/utils/exendian.cpp
	src/utils/logger.cpp
	src/utils/time.cpp
)

if(UNIX)
	set(PLATFORM_SRC
		src/epoll/EpollConnection.cpp
		src/epoll/EpollLoop.cpp
		src/epoll/EpollServer.cpp
	)
else()
	set(PLATFORM_SRC
		src/win32/net_win32.cpp
	)
endif()

# file(GLOB_RECURSE SOURCE_FILES src/*.h src/*.cpp )
set(SOURCE_FILES ${COMMON_FILES} ${PLATFORM_SRC})
add_library(meshy ${SOURCE_FILES})

add_executable(net_framework src/sample.cpp)
target_link_libraries(net_framework meshy)
