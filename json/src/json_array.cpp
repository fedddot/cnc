#include <stdexcept>
#include <sstream>

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
	std::stringstream output_string_stream;
	output_string_stream << (char)(JsonSpecialChar::ARRAY_START);
	for (auto iter = m_values.begin(); iter != m_values.end(); ++iter) {
		output_string_stream << (char)(JsonSpecialChar::SPACE) << (*iter)->getJsonString() << (char)(JsonSpecialChar::DELIMITER);
	}
	output_string_stream << (char)(JsonSpecialChar::ARRAY_END);
	return output_string_stream.str();
}

const char *JsonArray::parse(const char * const from) {
	if (nullptr == from) {
		throw std::invalid_argument("nullptr reveived");
	}
	const char *iter = skipChars(from, (char)(JsonSpecialChar::SPACE));
	if ((char)(JsonSpecialChar::ARRAY_START) != *iter) {
		throw std::invalid_argument("invalid array start");
	}
	++iter;
	JsonArray parsed_array;
	while (!('\0' == *iter)) {
		if ((char)(JsonSpecialChar::ARRAY_END) == *iter) {
			m_values = parsed_array.m_values;
			++iter;
			return iter;
		}

		iter = skipChars(iter, (char)(JsonSpecialChar::SPACE));
		
		std::shared_ptr<IJsonValue> new_value(nullptr);
		switch (*iter)
		{
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
			throw std::invalid_argument("failed to parse value");
		}

		iter = new_value->parse(iter);
		parsed_array.addObject(new_value);

		iter = skipChars(iter, (char)(JsonSpecialChar::SPACE));
		switch (*iter)
		{
		case ((char)(JsonSpecialChar::DELIMITER)):
			++iter;
			break;
		case ((char)(JsonSpecialChar::ARRAY_END)):
			;
			break;
		default:
			throw std::invalid_argument("failed to parse delimiter or object end");
		}
	}
	throw std::invalid_argument("invalid array end");
}

void JsonArray::addObject(const std::shared_ptr<IJsonValue>& value) {
	m_values.push_back(value);
}