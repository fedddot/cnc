#include <string>

#include "json_string.hpp"
#include "json_types.hpp"
#include "json_utils.hpp"

using namespace json;

JsonString::JsonValueType JsonString::getType() const {
	return JsonValueType::STRING;
}

std::string JsonString::getJsonString() const {
	std::string result("");
	result += (char)(JsonSpecialChar::STRING_START) + *this + (char)(JsonSpecialChar::STRING_END);
	return result;
}

std::vector<char>::iterator JsonString::parse(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end) {
	std::vector<char> skip_chars_list = {
		(char)(JsonSpecialChar::SPACE),
		(char)(JsonSpecialChar::TAB),
		(char)(JsonSpecialChar::NEW_LINE)
	};
	
	auto iter = skipChars(begin, end, skip_chars_list);
	if (iter == end) {
		return iter;
	}

	if ((char)(JsonSpecialChar::STRING_START) != *iter) {
		return iter;
	}
	++iter;

	std::string value("");
	while (end != iter) {
		if ((char)(JsonSpecialChar::STRING_END) == *iter) {
			*this = value;
			++iter;
			return iter;
		}
		value += *iter;
		++iter;
	}
	return iter;
}