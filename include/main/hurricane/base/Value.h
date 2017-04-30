#pragma once

#include <string>

namespace hurricane {
	namespace base {
		class Value {
		public:
			enum class Type
			{
				Boolean,
				Character,
				Int8,
				Int16,
				Int32,
				Int64,
				Float,
				Double,
				String,
			};

			union InnerValue
			{
				bool	booleanValue;
				char	characterValue;
				int8_t	int8Value;
				int16_t	int16Value;
				int32_t	int32Value;
				int64_t	int64Value;
				float	floatValue;
				double	doubleValue;
			};

			Value(bool value);
			Value(char value);
			Value(int8_t value);
			Value(int16_t value);
			Value(int32_t value);
			Value(int64_t value);
			Value(float value);
			Value(double value);
			Value(std::string const& value);

			char ToCharacter()const;
			bool ToBoolean()const;
			int8_t ToInt8()const;
			int16_t ToInt16()const;
			int32_t ToInt32()const;
			int64_t ToInt64()const;
			float ToFloat()const;
			double ToDouble()const;
			std::string const& ToString()const;

		private:
			Type		m_type;
			InnerValue	m_value;
			std::string	m_stringValue;
		};
	}
}