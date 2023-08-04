#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

#include "idata.hpp"
#include "string.hpp"
#include "array.hpp"
#include "object.hpp"

#include "json_common.hpp"
#include "json_parser.hpp"

using namespace json;
using namespace data;

std::shared_ptr<IData> JsonParser::parse(std::istream& data_stream) {
	skipBulks(data_stream);
	if (isEos(data_stream)) {
		throw JsonParserExceptionUnexpectedEof("JsonParser::parse");
	}
	char first_char = static_cast<char>(data_stream.peek());
	switch (first_char) {
	case static_cast<char>(JsonSpecialChar::STRING_START):
		return parseString(data_stream);
	case static_cast<char>(JsonSpecialChar::OBJECT_START):
		return parseObject(data_stream);
	case static_cast<char>(JsonSpecialChar::ARRAY_START):
		return parseArray(data_stream);
	default:
		break;
	}
	throw JsonParserExceptionUnexpectedToken("JsonParser::parse", data_stream.tellg(), "\" or { or [", std::string(&first_char, 1));
}

std::shared_ptr<data::IData> JsonParser::parseString(std::istream& data_stream) {
	char first_char = static_cast<char>(data_stream.peek());
	const char expected_first_char = static_cast<char>(JsonSpecialChar::STRING_START);
	if (expected_first_char != first_char) {
		JsonParserExceptionUnexpectedToken("JsonParser::parseString", data_stream.tellg(), std::string(&expected_first_char, 1), std::string(&first_char, 1));
	}

	data_stream.get();
	String parsed_string("");
	while (!isEos(data_stream)) {
		char curr_char = static_cast<char>(data_stream.peek());
		if (static_cast<char>(JsonSpecialChar::STRING_END) == curr_char) {
			data_stream.get();
			return std::shared_ptr<data::IData>(new String(parsed_string));
		}
		parsed_string += curr_char;
		data_stream.get();
	}
	throw JsonParserExceptionUnexpectedEof("JsonParser::parseString");
}

std::shared_ptr<data::IData> JsonParser::parseObject(std::istream& data_stream) {
	char first_char = static_cast<char>(data_stream.peek());
	const char expected_first_char = static_cast<char>(JsonSpecialChar::OBJECT_START);
	if (expected_first_char != first_char) {
		JsonParserExceptionUnexpectedToken("JsonParser::parseObject", data_stream.tellg(), std::string(&expected_first_char, 1), std::string(&first_char, 1));
	}

	data_stream.get();
	skipBulks(data_stream);
	Object parsed_object;
	while (!isEos(data_stream)) {
		char curr_char = static_cast<char>(data_stream.peek());
		if (static_cast<char>(JsonSpecialChar::OBJECT_END) == curr_char) {
			data_stream.get();
			return std::shared_ptr<data::IData>(new Object(parsed_object));
		}
		auto field_name_ptr = parseString(data_stream);
		String field_name(*field_name_ptr);
		skipBetweenFieldNameAndMember(data_stream);
		auto member_ptr = parse(data_stream);
		parsed_object[field_name] = member_ptr;
		skipAfterMember(data_stream, static_cast<char>(JsonSpecialChar::OBJECT_END));
	}
	throw JsonParserExceptionUnexpectedEof("JsonParser::parseObject");
}

std::shared_ptr<data::IData> JsonParser::parseArray(std::istream& data_stream) {
	char first_char = static_cast<char>(data_stream.peek());
	const char expected_first_char = static_cast<char>(JsonSpecialChar::ARRAY_START);
	if (expected_first_char != first_char) {
		JsonParserExceptionUnexpectedToken("JsonParser::parseArray", data_stream.tellg(), std::string(&expected_first_char, 1), std::string(&first_char, 1));
	}

	data_stream.get();
	skipBulks(data_stream);
	Array parsed_array;
	while (!data_stream.eof()) {
		char curr_char = static_cast<char>(data_stream.peek());
		if (static_cast<char>(JsonSpecialChar::ARRAY_END) == curr_char) {
			data_stream.get();
			return std::shared_ptr<data::IData>(new Array(parsed_array));
		}
		auto member_ptr = parse(data_stream);
		parsed_array.push_back(member_ptr);
		skipAfterMember(data_stream, static_cast<char>(JsonSpecialChar::ARRAY_END));
	}
	throw JsonParserExceptionUnexpectedEof("JsonParser::parseArray");
}

bool JsonParser::isEos(std::istream& data_stream) {
	return (EOF == data_stream.peek());
}

void JsonParser::skipChars(std::istream& data_stream, const std::vector<char>& chars) {
	while (!isEos(data_stream)) {
		auto iter = std::find(chars.begin(), chars.end(), data_stream.peek());
		if (chars.end() == iter) {
			return;
		}
		data_stream.get();
	}
}

void JsonParser::skipBulks(std::istream& data_stream) {
	const std::vector<char> bulk_chars {
		static_cast<char>(JsonSpecialChar::SPACE),
		static_cast<char>(JsonSpecialChar::TAB),
		static_cast<char>(JsonSpecialChar::NEW_LINE)
	};
	skipChars(data_stream, bulk_chars);
}

void JsonParser::skipBetweenFieldNameAndMember(std::istream& data_stream) {
	skipBulks(data_stream);
	const char token = static_cast<char>(data_stream.peek());
	const char expected_token = static_cast<char>(JsonSpecialChar::SEMICOLON);
	if (expected_token != token) {
		JsonParserExceptionUnexpectedToken("JsonParser::skipBetweenFieldNameAndMember", data_stream.tellg(), std::string(&expected_token, 1), std::string(&token, 1));
	}
	data_stream.get();
	skipBulks(data_stream);
}

void JsonParser::skipAfterMember(std::istream& data_stream, char expected_closing_bracket) {
	skipBulks(data_stream);
	const char token = static_cast<char>(data_stream.peek());
	if (static_cast<char>(JsonSpecialChar::DELIMITER) == token) {
		data_stream.get();
		skipBulks(data_stream);
		return;
	}

	if (expected_closing_bracket != token) {
		JsonParserExceptionUnexpectedToken("JsonParser::skipAfterMember", data_stream.tellg(), std::string(&expected_closing_bracket, 1), std::string(&token, 1));
	}
}