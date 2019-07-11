#pragma once

#include "socket.h"
#include "utils/common_utils.h"
#include "utils/logger.h"
#include <map>
#include <cassert>
#include <functional>

namespace meshy {
	
	/// 相同IP最大连接数
#define MAX_IP_COUNT 256

	class DataSink;
	class IStream;
	
	template<typename ConnectionType>
	class BasicServer : public Socket {
	public:
		typedef typename ConnectionType ConnectionType;

		typedef std::function<void(IStream*)> ConnectIndicationHandler;
		typedef std::function<void(IStream*)> DisconnectIndicationHandler;

		BasicServer(DataSink* dataSink = nullptr) : m_dataSink(dataSink) {}

	protected:
		/// 0成功，否则返回错误码
		int32_t bind(std::string const& host, uint16_t port) {
			NativeSocket listenfd = Socket::CreateNativeSocket();
			int32_t option = 1;
			setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option));
			NativeSocketAddress srvAddr = {0};
#ifdef OS_WIN32
			inet_pton(AF_INET, host.c_str(), &srvAddr.sin_addr);
#elif defined(OS_LINUX)
			SetNonBlocking(listenfd);	// windows 监听的socket 不要设置为非阻塞
			inet_aton(host.c_str(), &srvAddr.sin_addr);
#endif // OS_WIN32

			//srvAddr.sin_addr.s_addr = inet_addr(host.c_str());
			srvAddr.sin_family = AF_INET;
			srvAddr.sin_port = htons(port);
			int32_t errorCode = ::bind(listenfd, (sockaddr*)&srvAddr, sizeof(sockaddr));
			if (errorCode < 0)
			{
#ifdef OS_WIN32
				TRACE_ERROR("Bind failed. Error: {}", WSAGetLastError());
#else
				TRACE_ERROR("Bind failed. Error: {}", errno);
#endif
				assert(false);
				return errorCode;
			}
			SetNativeSocket(listenfd);
			SetNativeSocketAddress(srvAddr);
			return 0;
		}

	public:
		void OnConnectIndication(ConnectIndicationHandler handler){
			m_connectHandler = handler;
		}

		void OnDisconnectIndication(DisconnectIndicationHandler handler){
			m_disconnectIndication = handler;
		}
		/// 监听端口
		virtual int32_t listen(std::string const& host, uint16_t port, int backlog) = 0;
		/// 等待连接
		/// windows 这里传的是客户端的socket，linux不用传
		virtual ConnectionType accept(NativeSocket fd) = 0;

		void SetDataSink(DataSink* dataSink) { m_dataSink = dataSink; }
		DataSink* GetDataSink() { return m_dataSink; }

		ConnectionType GetConnection(NativeSocket fd) {
			auto it = m_connections.find(fd);
			return it == m_connections.end() ? nullptr : it->second;
		}

		int16_t IpCount(uint32_t ip) {
			return m_ipCount[ip];
		}
		int16_t ChangeIpCount(uint32_t ip, bool bAdd) {
			if (!ip)return 0; // 0不计入
			auto& curCount = m_ipCount[ip];
			curCount += bAdd ? 1 : -1;
			if (curCount < 0) {
				assert(false);
				curCount = 0;
			}
			return curCount;
		}

		ConnectionType find(NativeSocket fd) {
			std::unique_lock<std::mutex> lock(m_connectionsMutex);
			auto it = m_connections.find(fd);
			return it == m_connections.end() ? nullptr : it->second;
		}

		void insert(NativeSocket fd, ConnectionType connection) {
			std::unique_lock<std::mutex> lock(m_connectionsMutex);
			m_connections.insert({fd, connection });
		}

		void remove(NativeSocket fd) {
			std::unique_lock<std::mutex> lock(m_connectionsMutex);
			auto it = m_connections.find(fd);
			if (m_connections.end() == it)
				return;
			assert(it->second.get());
			if (m_disconnectIndication)
			{
				m_disconnectIndication(it->second.get());
			}
			m_connections.erase(it);
		}
	protected:
		ConnectIndicationHandler				m_connectHandler;
		DisconnectIndicationHandler				m_disconnectIndication;

		std::mutex								m_connectionsMutex;
		std::map<NativeSocket, ConnectionType>	m_connections;		
		std::map<uint32_t, int16_t>				m_ipCount;	// 连接的ip计数
	private:
		/// 每个服务器共享一个, 这里只是关联，不拥有内存所有权,可以是解包器
		DataSink*	m_dataSink;
	};	
}
