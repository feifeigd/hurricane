#pragma once

#include "../base/DataPackage.h"
#include <meshy.h>

namespace hurricane {
	namespace message {
		class Command {
		public:
			class Type {
			public:
				enum Values {
					Invalid = 0,
					Join,
					Alive,
					Leave,
					StartTopology,
					ShutdownTopology,
					StartSpout,
					StopSpout,
					RandomDestination,
					GroupDestination,
					Response,
					Data,
				};
			};

			Command() : m_type(Type::Invalid) {}
			Command(Type::Values type, base::Variants const& args) : m_type(type), m_args(args) {}

			Type::Values type()const { return m_type; }
			base::Variants const& args()const { return m_args; }
			std::shared_ptr<meshy::TcpConnection> src()const { return m_src; }

		private:
			Type::Values							m_type;
			base::Variants							m_args;
			std::shared_ptr<meshy::TcpConnection>	m_src;
		};
	}
}