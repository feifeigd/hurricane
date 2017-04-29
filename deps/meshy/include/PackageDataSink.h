#pragma once

#include "BaseEvent.h"
#include "ByteArray.h"
#include "DataSink.h"
#include "utils/thread_pool.h"

namespace meshy {

	class EventQueue;

	class PackageDataSink : public DataSink {
	public:
		PackageDataSink(EventQueue* eventQueue);
		~PackageDataSink();

		virtual size_t OnDataIndication(IStream* stream, char const* buf, size_t bytes)override;

	private:
		EventQueue*				m_eventQueue;
		ThreadPool<BaseEvent>	m_threadPool;
		ByteArray				m_data;
		size_t					m_totalSize;	///< 收到多少字节就算得到一个完整的数据包，需要处理
	};

}
