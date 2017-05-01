#pragma once

#include <cstdint>

namespace hurricane {
	namespace message {

		class Message {
		public:
			struct Type
			{
				enum {
					Stop = 0,
				};
			};

			Message(int32_t type) : m_type(type) {}

			int32_t type()const { return m_type; }
			void type(int32_t type) { m_type = type; }

		private:
			int32_t	m_type;
		};
	}
}
