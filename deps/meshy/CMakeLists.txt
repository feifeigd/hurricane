

set(COMMON_FILES
	include/meshy.h
	include/BaseEvent.h
	include/BasicServer.hpp
	include/BasicStream.h
	include/ByteArray.h
	include/ByteArrayReader.h
	include/ByteArrayWriter.h
	include/EventQueue.h
	include/EventQueueLoop.h
	include/http/HttpConnection.h
	include/http/HttpContext.h
	include/http/HttpRequest.h
	include/http/HttpResponse.h
	include/http/HttpServer.h
	include/IConnectable.h
	include/loop.h
	include/PackageDataSink.h
	include/socket.h
	include/utils/common_utils.h
	include/utils/exendian.h
	include/utils/logger.h
	include/utils/String.h
	include/utils/time.h
	src/BaseEvent.cpp
	src/BasicStream.cpp
	src/ByteArray.cpp
	src/ByteArrayReader.cpp
	src/ByteArrayWriter.cpp
	src/EventQueue.cpp
	src/EventQueueLoop.cpp
	src/http/HttpConnection.cpp
	src/http/HttpContext.cpp
	src/http/HttpRequest.cpp
	src/http/HttpResponse.cpp
	src/http/HttpServer.cpp
	src/loop.cpp
	src/PackageDataSink.cpp
	src/socket.cpp
	src/utils/common_utils.cpp
	src/utils/exendian.cpp
	src/utils/logger.cpp
	src/utils/String.cpp
	src/utils/time.cpp
)

if(UNIX)
	set(PLATFORM_SRC
		src/epoll/EpollClient.cpp
		src/epoll/EpollConnection.cpp
		src/epoll/EpollLoop.cpp
		src/epoll/EpollServer.cpp
		src/epoll/EpollStream.cpp
	)
else()
	set(PLATFORM_SRC	
		include/iocp/IocpLoop.h
		include/iocp/IocpClient.h
		include/iocp/IocpServer.h
		include/iocp/IocpStream.h
		src/iocp/Iocp.cpp
		src/iocp/IocpLoop.cpp
		src/iocp/IocpClient.cpp
		src/iocp/IocpServer.cpp
		src/iocp/IocpStream.cpp
		src/iocp/WSAConnection.cpp
		src/win32/net_win32.cpp
	)
endif()

# file(GLOB_RECURSE SOURCE_FILES src/*.h src/*.cpp )
set(SOURCE_FILES ${COMMON_FILES} ${PLATFORM_SRC})
add_library(meshy ${SOURCE_FILES})
target_link_libraries(meshy fmt::fmt)

add_executable(net_framework src/sample.cpp)
target_link_libraries(net_framework meshy)

if(UNIX)
	target_link_libraries(net_framework pthread)
else()
	target_link_libraries(net_framework Mswsock.lib)	
endif()

