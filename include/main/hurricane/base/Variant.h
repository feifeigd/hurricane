#pragma once

#include <vector>

namespace hurricane {
	namespace base {
		class Variant {
		public:
			enum class Type {
				Invalid,
				Integer,
				Boolean,
				Float,
				String,
			};

			Variant() : m_type(Type::Invalid) {}

			std::string const& GetStringValue()const;

		private:
			Type	m_type;
			union {
				int32_t	m_intValue;
				bool	m_boolValue;
				float	m_floatValue;
			};
			std::string	m_stringValue;
		};

		typedef std::vector<Variant>	Variants;
	}
}