#include <hurricane/base/Variant.h>
#include <iostream>

using hurricane::base::Variant;

std::string const& Variant::GetStringValue()const {
	if (Type::Invalid == m_type)std::cerr << "Invalid";
	if (Type::String == m_type)return m_stringValue;
	throw "Type mismatched";
}
