#include <sstream>

#include "json_common.hpp"

using namespace json;

JsonParserExceptionUnexpectedCharacter::JsonParserExceptionUnexpectedCharacter(const std::string& where, const std::size_t& pos_in_stream, char expected, char received) : m_where(where), m_pos_in_stream(pos_in_stream), m_expected(expected), m_received(received), m_msg("") {
	std::stringstream msg_stream;
	msg_stream << "In \"" << where << "\": unexpected character received (\'" << received << "\') while \'" << expected << "\' expected (position in input stream = " << pos_in_stream << ")";
	m_msg = msg_stream.str();
}