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
			DataBuffSize = BUFSIZ,
		};

		class OperationType {
		public:
			enum {
				Read,
				Write,
			};
		};
		struct OperationData {
			OVERLAPPED	overlapped;
			WSABUF		databuff;
			char		buffer[DataBuffSize];
			int			operationType;
			IocpStream*	stream;
		};
		typedef std::shared_ptr<OperationData> OperationDataPtr;

		static HANDLE GetCompletionPort();
		static OperationDataPtr CreateOperationData(IocpStreamPtr stream, HANDLE completionPort);
		static void ResetOperationData(OperationData* perIoData);
	};
}
