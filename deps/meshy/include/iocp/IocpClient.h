#pragma once

#include "../IConnectable.h"
#include "../win32/net_win32.h"
#include "IocpStream.h"

namespace meshy {
	class IocpClient;
	typedef std::shared_ptr<IocpClient>	IocpClientPtr;

	class IocpClient : public IocpStream, public IConnectable {
		IocpClient(IocpClient const&) = delete;
	public:
		virtual void connect(std::string const& host, uint16_t port)override;
		void reconnect();
		static IocpClientPtr Connect(std::string const& host, uint16_t port);

	private:
		IocpClient(NativeSocket socket);
	};
}