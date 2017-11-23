#pragma once

#include "../win32/net_win32.h"
#include <memory>

namespace meshy {

	class IocpStream;
	typedef std::shared_ptr<IocpStream> IocpStreamPtr;

	class Iocp
	{
	public:
		enum {
			DataBuffSize = 8*1024,	// 8K
		};

		class OperationType {
		public:
			enum {
				Read,
				Write,
				Accept,
			};
		};
		struct OperationData {
			OVERLAPPED	overlapped;
			WSABUF		databuff;
			char		buffer[DataBuffSize];
			int			operationType;
			IocpStream*	stream;
		};
		//typedef std::shared_ptr<OperationData> OperationDataPtr;

		/// 创建完成端口
		static HANDLE CreateCompletionPort();
		static OperationData& CreateOperationData(IocpStreamPtr stream, HANDLE completionPort);
		static void ResetOperationData(OperationData* perIoData);
	};
}
