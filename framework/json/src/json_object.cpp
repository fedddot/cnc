#include <map>
#include <vector>
#include <string>
#include <memory>
#include "json_object.hpp"
#include "json_string.hpp"
#include "json_array.hpp"
#include "json_types.hpp"
#include "json_utils.hpp"

using namespace json;

JsonObject::JsonValueType JsonObject::getType() const {
	return JsonValueType::OBJECT;
}

std::string JsonObject::getJsonString() const {
	std::string output_string("");
	output_string += (char)(JsonSpecialChar::OBJECT_START);
	auto iter = begin();
	while (end() != iter) {
		output_string += (char)(JsonSpecialChar::STRING_START); 
		output_string += iter->first;
		output_string += (char)(JsonSpecialChar::STRING_END);
		output_string += (char)(JsonSpecialChar::SEMICOLON);
		output_string += (char)(JsonSpecialChar::SPACE);
		output_string += iter->second->getJsonString();
		++iter;
		if (end() != iter) {
			output_string += (char)(JsonSpecialChar::DELIMITER);
			output_string += (char)(JsonSpecialChar::SPACE);
		}
	}
	output_string += (char)(JsonSpecialChar::OBJECT_END);
	return output_string;
}

std::vector<char>::iterator JsonObject::parse(const std::vector<char>::iterator& begin, const std::vector<char>::iterator& end) {
	std::vector<char> skip_chars_list = {
		(char)(JsonSpecialChar::SPACE),
		(char)(JsonSpecialChar::TAB),
		(char)(JsonSpecialChar::NEW_LINE)
	};
	
	auto iter = skipChars(begin, end, skip_chars_list);
	if (iter == end) {
		return iter;
	}

	if ((char)(JsonSpecialChar::OBJECT_START) != *iter) {
		return iter;
	}
	++iter;

	JsonObject value;
	while (end != iter) {
		iter = skipChars(iter, end, skip_chars_list);
		if (iter == end) {
			return iter;
		}

		if ((char)(JsonSpecialChar::OBJECT_END) == *iter) {
			*this = value;
			++iter;
			return iter;
		}

		JsonString field_name("");
		iter = field_name.parse(iter, end);
		if (0 == field_name.size()) {
			return iter;
		}

		iter = skipChars(iter, end, skip_chars_list);
		if (iter == end) {
			return iter;
		}

		if ((char)(JsonSpecialChar::SEMICOLON) != *iter) {
			return iter;
		}
		++iter;

		iter = skipChars(iter, end, skip_chars_list);
		if (iter == end) {
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
		(*this)[field_name] = new_value;
		
		iter = skipChars(iter, end, skip_chars_list);
		if (iter == end) {
			return iter;
		}

		switch (*iter)
		{
		case ((char)(JsonSpecialChar::DELIMITER)):
			++iter;
			break;
		case ((char)(JsonSpecialChar::OBJECT_END)):
			;
			break;
		default:
			return iter;
		}
	}
	return iter;
}
