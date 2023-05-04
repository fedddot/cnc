#include <stdexcept>
#include <sstream>

#include "json_string.hpp"
#include "json_types.hpp"
#include "json_utils.hpp"

using namespace json;

JsonString::JsonString(const std::string& value) : m_value(value) {

}

JsonString::JsonValueType JsonString::getType() const {
	return JsonValueType::STRING;
}

std::string JsonString::getJsonString() const {
	std::stringstream output_string_stream;
	
	output_string_stream << (char)(JsonSpecialChar::STRING_START) << m_value << (char)(JsonSpecialChar::STRING_END);
	return output_string_stream.str();
}

const char *JsonString::parse(const char * const from) {
	if (nullptr == from) {
		throw std::invalid_argument("nullptr reveived");
	}
	const char *iter = skipChars(from, (char)(JsonSpecialChar::SPACE));
	if ((char)(JsonSpecialChar::STRING_START) != *iter) {
		throw std::invalid_argument("invalid string start");
	}
	++iter;
	std::stringstream value_stream;
	while (!('\0' == *iter)) {
		if ((char)(JsonSpecialChar::STRING_END) == *iter) {
			m_value = value_stream.str();
			++iter;
			return iter;
		}
		value_stream << *iter;
		++iter;
	}
	throw std::invalid_argument("invalid string end");
}