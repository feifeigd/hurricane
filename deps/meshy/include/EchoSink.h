#pragma once

#include "DataSink.h"
#include <string>
#include <cassert>
#include "utils/logger.h"

class EchoSink : public meshy::DataSink{
public:
	/// 收到了对端的一段数据
	virtual size_t OnDataIndication(meshy::IStream* stream, char const* buf, size_t bytes){
		assert(stream && buf);
		//TRACE_DEBUG("Receive Data:%s", buf);
		return stream->send({ buf, bytes });		
	}
};
