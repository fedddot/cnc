#include <stdexcept>
#include <sstream>

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
	std::stringstream output_string_stream;
	output_string_stream << (char)(JsonSpecialChar::OBJECT_START);
	for (auto iter = m_values.begin(); iter != m_values.end(); ++iter) {
		output_string_stream << (char)(JsonSpecialChar::SPACE) << (char)(JsonSpecialChar::STRING_START) << iter->first << (char)(JsonSpecialChar::STRING_END) << (char)(JsonSpecialChar::SEMICOLON) << (char)(JsonSpecialChar::SPACE) << iter->second->getJsonString() << (char)(JsonSpecialChar::DELIMITER);
	}
	output_string_stream << (char)(JsonSpecialChar::OBJECT_END);
	return output_string_stream.str();
}

const char *JsonObject::parse(const char * const from) {
	if (nullptr == from) {
		throw std::invalid_argument("nullptr reveived");
	}
	const char *iter = skipChars(from, (char)(JsonSpecialChar::SPACE));
	if ((char)(JsonSpecialChar::OBJECT_START) != *iter) {
		throw std::invalid_argument("invalid object start");
	}
	++iter;
	JsonObject parsed_object;
	while (!('\0' == *iter)) {
		if ((char)(JsonSpecialChar::OBJECT_END) == *iter) {
			m_values = parsed_object.m_values;
			++iter;
			return iter;
		}

		JsonString field_name;
		iter = field_name.parse(iter);

		iter = skipChars(iter, (char)(JsonSpecialChar::SPACE));
		if ((char)(JsonSpecialChar::SEMICOLON) != *iter) {
			throw std::invalid_argument("failed to parse key");
		}
		++iter;

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
		parsed_object.addObject(field_name.get(), new_value);

		iter = skipChars(iter, (char)(JsonSpecialChar::SPACE));
		switch (*iter)
		{
		case ((char)(JsonSpecialChar::DELIMITER)):
			++iter;
			break;
		case ((char)(JsonSpecialChar::OBJECT_END)):
			;
			break;
		default:
			throw std::invalid_argument("failed to parse delimiter or object end");
		}
	}
	throw std::invalid_argument("invalid string end");
}

void JsonObject::addObject(const std::string& key, const std::shared_ptr<IJsonValue>& value) {
	m_values[key] = value;
}