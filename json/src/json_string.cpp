#include "json_string.hpp"
#include "json_types.hpp"
#include "json_utils.hpp"

#include "string.hpp"

using namespace json;

JsonString::JsonString(const data::String& value) : m_value(value) {

}

JsonString::JsonValueType JsonString::getType() const {
	return JsonValueType::STRING;
}

data::String JsonString::getJsonString() const {
	data::String output_string(data::String((char)(JsonSpecialChar::STRING_START)) + m_value + data::String((char)(JsonSpecialChar::STRING_END)));
	return output_string;
}

const char *JsonString::parse(const char * const from) {
	if (nullptr == from) {
		return nullptr;
	}
	const char *iter = skipChars(from, (char)(JsonSpecialChar::SPACE));
	if ((char)(JsonSpecialChar::STRING_START) != *iter) {
		return nullptr;
	}
	++iter;
	data::String value("");
	while (!('\0' == *iter)) {
		if ((char)(JsonSpecialChar::STRING_END) == *iter) {
			m_value = value;
			++iter;
			return iter;
		}
		value = value + data::String((char)(*iter));
		++iter;
	}
	throw nullptr;
}