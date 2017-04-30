
#include <hurricane/base/TypeMismatchExcepton.h>
#include <hurricane/base/Value.h>

using hurricane::base::Value;
using hurricane::base::TypeMismatchException;

Value::Value(bool value) : m_type(Type::Boolean) {
	m_value.booleanValue = value;
}

Value::Value(char value) : m_type(Type::Character) {
	m_value.characterValue = value;
}

Value::Value(int8_t value) : m_type(Type::Int8) {
	m_value.int8Value = value;
}

Value::Value(int16_t value) : m_type(Type::Int16) {
	m_value.int16Value = value;
}

Value::Value(int32_t value) : m_type(Type::Int32) {
	m_value.int32Value = value;
}

Value::Value(int64_t value) : m_type(Type::Int64) {
	m_value.int64Value = value;
}

Value::Value(float value) : m_type(Type::Float) {
	m_value.floatValue = value;
}

Value::Value(double value) : m_type(Type::Double) {
	m_value.doubleValue = value;
}

Value::Value(std::string const& value) : m_type(Type::String) {
	m_stringValue = value;
}

char Value::ToCharacter()const {
	if (Type::Character != m_type)throw TypeMismatchException("The type of value is not character.");
	return m_value.characterValue;
}

bool Value::ToBoolean()const {
	if (Type::Boolean != m_type)throw TypeMismatchException("The type of value is not boolean.");
	return m_value.booleanValue;
}

int8_t Value::ToInt8()const {
	if (Type::Int8 != m_type)throw TypeMismatchException("The type of value is not int8.");
	return m_value.int8Value;
}

int16_t Value::ToInt16()const {
	if (Type::Int16 != m_type)throw TypeMismatchException("The type of value is not int16.");
	return m_value.int16Value;
}

int32_t Value::ToInt32()const {
	if (Type::Int32 != m_type)throw TypeMismatchException("The type of value is not int32.");
	return m_value.int32Value;
}

int64_t Value::ToInt64()const {
	if (Type::Boolean != m_type)throw TypeMismatchException("The type of value is not int64.");
	return m_value.int64Value;
}

float Value::ToFloat()const {
	if (Type::Float != m_type)throw TypeMismatchException("The type of value is not float.");
	return m_value.floatValue;
}

double Value::ToDouble()const {
	if (Type::Double != m_type)throw TypeMismatchException("The type of value is not double.");
	return m_value.doubleValue;
}

std::string const& Value::ToString()const {
	if (Type::String != m_type)throw TypeMismatchException("The type of value is not string.");
	return m_stringValue;
}
