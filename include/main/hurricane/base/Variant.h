#pragma once

#include <map>
#include <string>
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

			static std::map<Type, int8_t> TypeCodes;
			Variant() : m_type(Type::Invalid) {}
			Variant(int32_t intValue) : m_type(Type::Integer), m_intValue(intValue){}
			Variant(bool boolValue) : m_type(Type::Boolean), m_boolValue(boolValue) {}
			Variant(std::string const& value) : m_type(Type::String), m_stringValue(value){}

			Variant(Variant const& variant);
			Variant const& operator=(Variant const& variant);

			Type type()const { return m_type; }

			int32_t GetIntValue()const;
			void SetIntValue(int32_t value);

			std::string const& GetStringValue()const;

		private:
			Type	m_type;
			union {
				int32_t	m_intValue = 0;
				bool	m_boolValue;
				float	m_floatValue;
			};
			std::string	m_stringValue;
		};

		typedef std::vector<Variant>	Variants;
	}
}