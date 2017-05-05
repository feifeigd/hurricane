#pragma once

#include "../base/NetAddress.h"
#include "../base/NetConnector.h"

namespace hurricane {

	namespace base { class Values; }

	namespace message {
		class SupervisorCommander {
		public:
			SupervisorCommander(base::NetAddress const& nimbusAddress, std::string const& supervisorName);

			void connect();
			void join();
			void alive();

			void SendTuple(int taskIndex, base::Values const& values);

			void RandomDestination();

		private:
			base::NetAddress				m_nimbusAddress;
			std::string						m_supervisorName;
			std::shared_ptr<NetConnector>	m_connector;
		};
	}
}