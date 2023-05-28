#include <vector>
#include <string>
#include <memory>

#include "json_object.hpp"
#include "json_string.hpp"
#include "json_array.hpp"
#include "json_types.hpp"
#include "json_utils.hpp"

using namespace json;

JsonArray::JsonValueType JsonArray::getType() const {
	return JsonValueType::ARRAY;
}

std::string JsonArray::getJsonString() const {
	std::string output_string("");
	output_string += (char)(JsonSpecialChar::ARRAY_START);
	auto iter = begin();
	while (end() != iter) {
		output_string += (*iter)->getJsonString();
		++iter;
		if (end() != iter) {
			output_string += (char)(JsonSpecialChar::DELIMITER);
			output_string += (char)(JsonSpecialChar::SPACE);
		}
	}
	output_string += (char)(JsonSpecialChar::ARRAY_END);
	return output_string;
}

std::vector<char>::iterator JsonArray::parse(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end) {
	std::vector<char> skip_chars_list = {
		(char)(JsonSpecialChar::SPACE),
		(char)(JsonSpecialChar::TAB),
		(char)(JsonSpecialChar::NEW_LINE)
	};
	
	auto iter = skipChars(begin, end, skip_chars_list);
	if (iter == end) {
		return iter;
	}

	if ((char)(JsonSpecialChar::ARRAY_START) != *iter) {
		return iter;
	}
	++iter;

	JsonArray value;
	while (end != iter) {
		iter = skipChars(iter, end, skip_chars_list);
		if (iter == end) {
			return iter;
		}

		if ((char)(JsonSpecialChar::ARRAY_END) == *iter) {
			*this = value;
			++iter;
			return iter;
		}
		
		std::shared_ptr<IJsonValue> new_value(nullptr);
		switch (*iter) {
		case ((char)(JsonSpecialChar::STRING_START)):
			new_value = std::shared_ptr<IJsonValue>(new JsonString());
			break;
		case ((char)(JsonSpecialChar::OBJECT_START)):
			new_value = std::shared_ptr<IJsonValue>(new JsonObject());
			break;
		case ((char)(JsonSpecialChar::ARRAY_START)):
			new_value = std::shared_ptr<IJsonValue>(new JsonArray());
			break;		
		default:
			return iter;
		}

		iter = new_value->parse(iter, end);
		value.push_back(new_value);

		iter = skipChars(iter, end, skip_chars_list);
		if (iter == end) {
			return iter;
		}

		switch (*iter) {
		case ((char)(JsonSpecialChar::DELIMITER)):
			++iter;
			break;
		case ((char)(JsonSpecialChar::ARRAY_END)):
			;
			break;
		default:
			return iter;
		}
	}
	return iter;
}