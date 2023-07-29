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
	return nullptr;
}

std::shared_ptr<data::IData> JsonParser::parseString(std::istream& data_stream) {
	char first_char = peekChar(data_stream);
	if (static_cast<char>(JsonSpecialChar::STRING_START) != first_char) {
		throw JsonParserExceptionUnexpectedCharacter("JsonParser::parseString", data_stream.tellg(), static_cast<char>(JsonSpecialChar::STRING_START), first_char);
	}

	popChar(data_stream);
	String parsed_string("");
	while (!data_stream.eof()) {
		char curr_char = popChar(data_stream);
		if (static_cast<char>(JsonSpecialChar::STRING_END) == curr_char) {
			popChar(data_stream);
			return std::shared_ptr<data::IData>(new String(parsed_string));
		}
		parsed_string += curr_char;
	}
	throw JsonParserExceptionUnexpectedEof("JsonParser::parseString", static_cast<char>(JsonSpecialChar::STRING_END));
}