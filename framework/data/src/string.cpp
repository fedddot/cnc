#include <string>
#include <stdexcept>
#include "idata.hpp"
#include "string.hpp"

using namespace data;

String::String(const String& other): ValueType(other) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
}

String& String::operator=(const String& other) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
	static_cast<ValueType&>(*this) = other;
	return *this;
}

String::String(const ValueType& other): ValueType(other) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
}

String& String::operator=(const ValueType& other) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
	static_cast<ValueType&>(*this) = other;
	return *this;
}

String::String(const IData& other): ValueType(dynamic_cast<const String&>(other)) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
}

String& String::operator=(const IData& other) {
	// TODO: implement here element-wise copying of the members (not just a shallow copy of their ptrs)
	*this = dynamic_cast<const String&>(other);
	return *this;
}