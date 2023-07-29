#include <iostream>
#include <memory>

#include "idata.hpp"
#include "string.hpp"
#include "array.hpp"
#include "object.hpp"

#include "idata_parser.hpp"

#include "json_common.hpp"
#include "json_parser.hpp"

using namespace json;
using namespace data;

std::shared_ptr<IData> JsonParser::parse(std::istream& data_stream) {
	skipBulks(data_stream);
	char first_char = peekChar(data_stream);
	switch (first_char) {
	case static_cast<char>(JsonSpecialChar::STRING_START):
		return parseString(data_stream);
	case static_cast<char>(JsonSpecialChar::OBJECT_START):
		return parseObject(data_stream);
	case static_cast<char>(JsonSpecialChar::ARRAY_START):
		return parseArray(data_stream);
	default:
		JsonParserExceptionUnexpectedToken("JsonParser::parse", data_stream.tellg(), "\" or { or [", std::string(&first_char, 1));
	}
	return nullptr;
}

std::shared_ptr<data::IData> JsonParser::parseString(std::istream& data_stream) {
	char first_char = peekChar(data_stream);
	const char expected_first_char = static_cast<char>(JsonSpecialChar::STRING_START);
	if (expected_first_char != first_char) {
		JsonParserExceptionUnexpectedToken("JsonParser::parseString", data_stream.tellg(), std::string(&expected_first_char, 1), std::string(&first_char, 1));
	}

	popChar(data_stream);
	String parsed_string("");
	while (!data_stream.eof()) {
		char curr_char = peekChar(data_stream);
		if (static_cast<char>(JsonSpecialChar::STRING_END) == curr_char) {
			popChar(data_stream);
			return std::shared_ptr<data::IData>(new String(parsed_string));
		}
		parsed_string += curr_char;
		popChar(data_stream);
	}
	throw JsonParserExceptionUnexpectedEof("JsonParser::parseString", static_cast<char>(JsonSpecialChar::STRING_END));
}

std::shared_ptr<data::IData> JsonParser::parseObject(std::istream& data_stream) {
	char first_char = peekChar(data_stream);
	const char expected_first_char = static_cast<char>(JsonSpecialChar::OBJECT_START);
	if (expected_first_char != first_char) {
		JsonParserExceptionUnexpectedToken("JsonParser::parseObject", data_stream.tellg(), std::string(&expected_first_char, 1), std::string(&first_char, 1));
	}

	popChar(data_stream);
	Object parsed_object;
	while (!data_stream.eof()) {
		skipBulks(data_stream);
		char curr_char = peekChar(data_stream);
		if (static_cast<char>(JsonSpecialChar::OBJECT_END) == curr_char) {
			popChar(data_stream);
			return std::shared_ptr<data::IData>(new Object(parsed_object));
		}
		auto field_name_ptr = parseString(data_stream);
		String field_name(*field_name_ptr);
		skipBetweenFieldNameAndMember(data_stream);
		auto member_ptr = parse(data_stream);

		parsed_object[field_name] = member_ptr;
		skipAfterMember(data_stream, static_cast<char>(JsonSpecialChar::OBJECT_END));
	}
	throw JsonParserExceptionUnexpectedEof("JsonParser::parseObject", static_cast<char>(JsonSpecialChar::OBJECT_END));
}

std::shared_ptr<data::IData> JsonParser::parseArray(std::istream& data_stream) {
	char first_char = peekChar(data_stream);
	const char expected_first_char = static_cast<char>(JsonSpecialChar::ARRAY_START);
	if (expected_first_char != first_char) {
		JsonParserExceptionUnexpectedToken("JsonParser::parseArray", data_stream.tellg(), std::string(&expected_first_char, 1), std::string(&first_char, 1));
	}

	popChar(data_stream);
	Array parsed_array;
	while (!data_stream.eof()) {
		skipBulks(data_stream);
		char curr_char = peekChar(data_stream);
		if (static_cast<char>(JsonSpecialChar::ARRAY_END) == curr_char) {
			popChar(data_stream);
			return std::shared_ptr<data::IData>(new Array(parsed_array));
		}
		auto member_ptr = parse(data_stream);

		parsed_array.push_back(member_ptr);
		skipAfterMember(data_stream, static_cast<char>(JsonSpecialChar::ARRAY_END));
	}
	throw JsonParserExceptionUnexpectedEof("JsonParser::parseArray", static_cast<char>(JsonSpecialChar::ARRAY_END));
}