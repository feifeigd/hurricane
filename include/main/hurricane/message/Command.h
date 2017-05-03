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
			Command(base::DataPackage const& dataPackage);

			base::DataPackage ToDataPackage()const;

			Type::Values type()const { return m_type; }
			base::Variants const& args()const { return m_args; }
			meshy::IStream* src()const { return m_src; }
			void src(meshy::IStream* s) { m_src = s; }

		private:
			Type::Values	m_type;
			base::Variants	m_args;
			meshy::IStream*	m_src;
		};
	}
}