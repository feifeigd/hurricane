#include <hurricane/base/Variant.h>
#include <iostream>

using hurricane::base::Variant;

std::string const& Variant::GetStringValue()const {
	if (Type::Invalid == m_type)std::cerr << "Invalid";
	if (Type::String == m_type)return m_stringValue;
	throw "Type mismatched";
}

Variant::Variant(Variant const& variant) : m_type(variant.m_type)
{
	if (Type::Integer == m_type)m_intValue = variant.m_intValue;
	else if (Type::String == m_type)m_stringValue = variant.m_stringValue;
}

Variant const& Variant::operator=(Variant const& variant) {
	m_type = variant.m_type;
	if (Type::Integer == m_type)m_intValue = variant.m_intValue;
	else if (Type::String == m_type)m_stringValue = variant.m_stringValue;
	return *this;
}

int32_t Variant::GetIntValue()const {
	if (Type::Invalid == m_type)std::cerr << "Invalid";
	else if (Type::Integer == m_type)return m_intValue;
	throw "Type mismatched";
}

void Variant::SetIntValue(int32_t value) {
	m_type = Type::Integer;
	m_intValue = value;
}

std::map<Variant::Type, int8_t> Variant::TypeCodes = {
	{Variant::Type::Integer, 0},
	{Variant::Type::String, 3}
};